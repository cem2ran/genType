/***
 * Copyright 2004-present Facebook. All Rights Reserved.
 */

module StringSet = Set.Make(String);

open GenFlowCommon;

let getPriority = x =>
  switch (x) {
  | CodeItem.ImportType(_)
  | ExternalReactClass(_) => "2low"
  | ValueBinding(_)
  | ConstructorBinding(_)
  | ComponentBinding(_)
  | ExportType(_)
  | ExportVariantType(_) => "1med"
  };

let sortcodeItemsByPriority = codeItems => {
  module M = StringMap;
  let map =
    codeItems
    |> List.fold_left(
         (map, codeItem) => {
           let priority = codeItem |> getPriority;
           let items =
             try (map |> StringMap.find(priority)) {
             | Not_found => []
             };
           map |> StringMap.add(priority, [codeItem, ...items]);
         },
         StringMap.empty,
       );
  let sortedCodeItems = ref([]);
  map
  |> StringMap.iter((_priority, codeItemsAtPriority) =>
       codeItemsAtPriority
       |> List.iter(codeItem =>
            sortedCodeItems := [codeItem, ...sortedCodeItems^]
          )
     );
  sortedCodeItems^;
};

let hasGenFlowAnnotation = attributes =>
  CodeItem.getGenFlowKind(attributes) != NoGenFlow;

let typedItemHasGenFlowAnnotation = typedItem =>
  switch (typedItem) {
  | {Typedtree.str_desc: Typedtree.Tstr_type(typeDeclarations), _} =>
    typeDeclarations
    |> List.exists(dec => dec.Typedtree.typ_attributes |> hasGenFlowAnnotation)
  | {Typedtree.str_desc: Tstr_value(_loc, valueBindings), _} =>
    valueBindings
    |> List.exists(vb => vb.Typedtree.vb_attributes |> hasGenFlowAnnotation)
  | {Typedtree.str_desc: Tstr_primitive(valueDescription), _} =>
    valueDescription.val_attributes |> hasGenFlowAnnotation
  | _ => false
  };

let translateTypedItem =
    (~language, ~propsTypeGen, ~moduleName, typedItem): CodeItem.translation =>
  switch (typedItem) {
  | {Typedtree.str_desc: Typedtree.Tstr_type(typeDeclarations), _} =>
    typeDeclarations
    |> List.map(CodeItem.translateTypeDecl(~language))
    |> CodeItem.combineTranslations

  | {Typedtree.str_desc: Tstr_value(_loc, valueBindings), _} =>
    valueBindings
    |> List.map(
         CodeItem.translateValueBinding(
           ~language,
           ~propsTypeGen,
           ~moduleName,
         ),
       )
    |> CodeItem.combineTranslations

  | {Typedtree.str_desc: Tstr_primitive(valueDescription), _} =>
    /* external declaration */
    valueDescription |> CodeItem.translateValueDescription(~language)

  | _ => {CodeItem.dependencies: [], CodeItem.codeItems: []}
  /* TODO: Support mapping of variant type definitions. */
  };

let cmtHasGenFlowAnnotations = inputCMT =>
  switch (inputCMT.Cmt_format.cmt_annots) {
  | Implementation(structure) =>
    structure.Typedtree.str_items
    |> List.exists(typedItemHasGenFlowAnnotation)
  | _ => false
  };

let cmtToCodeItems =
    (
      ~config,
      ~propsTypeGen,
      ~moduleName,
      ~outputFileRelative,
      ~resolver,
      inputCMT,
    )
    : list(CodeItem.t) => {
  let {Cmt_format.cmt_annots, _} = inputCMT;
  switch (cmt_annots) {
  | Implementation(structure) =>
    let typedItems = structure.Typedtree.str_items;
    let translationUnit =
      typedItems
      |> List.map(nextTypedItem =>
           nextTypedItem
           |> translateTypedItem(
                ~language=config.language,
                ~propsTypeGen,
                ~moduleName,
              )
         )
      |> CodeItem.combineTranslations;
    let codeItems = translationUnit.codeItems;
    let imports =
      translationUnit.dependencies
      |> CodeItem.translateDependencies(
           ~config,
           ~outputFileRelative,
           ~resolver,
         );
    imports @ (codeItems |> sortcodeItemsByPriority);
  | _ => []
  };
};

let emitCodeItems =
    (
      ~language,
      ~outputFile,
      ~outputFileRelative,
      ~signFile,
      ~resolver,
      codeItems,
    ) => {
  let codeText =
    codeItems
    |> EmitJs.emitCodeItems(~language, ~outputFileRelative, ~resolver);
  let fileContents =
    signFile(EmitTyp.fileHeader(~language) ++ "\n" ++ codeText);

  GeneratedFiles.writeFileIfRequired(~fileName=outputFile, ~fileContents);
};

let processCmtFile = (~signFile, ~config, cmt) => {
  let cmtFile = Filename.concat(Sys.getcwd(), cmt);
  if (Sys.file_exists(cmtFile)) {
    let propsTypeGen = GenIdent.createPropsTypeGen();
    let inputCMT = Cmt_format.read_cmt(cmtFile);
    let outputFile = cmt |> Paths.getOutputFile(~language=config.language);
    let outputFileRelative =
      cmt |> Paths.getOutputFileRelative(~language=config.language);
    let moduleName = cmt |> Paths.getModuleName;
    let resolver =
      ModuleResolver.createResolver(
        ~extensions=[
          ".re",
          EmitTyp.shimExtension(~language=config.language),
        ],
      );
    if (inputCMT |> cmtHasGenFlowAnnotations) {
      inputCMT
      |> cmtToCodeItems(
           ~config,
           ~propsTypeGen,
           ~moduleName,
           ~outputFileRelative,
           ~resolver,
         )
      |> emitCodeItems(
           ~language=config.language,
           ~outputFile,
           ~outputFileRelative,
           ~signFile,
           ~resolver,
         );
    } else {
      outputFile |> GeneratedFiles.logFileAction(NoMatch);
      if (Sys.file_exists(outputFile)) {
        Unix.unlink(outputFile);
      };
    };
  };
};