// Generated by BUCKLESCRIPT VERSION 4.0.7, PLEASE EDIT WITH CARE


var y = "abc";

var EmptyInnerModule = /* module */[];

var InnerModule2 = /* module */[/* k */4242];

var Z = /* module */[/* u : tuple */[
    0,
    0
  ]];

var M = /* module */[
  /* y */y,
  /* EmptyInnerModule */EmptyInnerModule,
  /* InnerModule2 */InnerModule2,
  /* Z */Z,
  /* x */42
];

var firstClassModule = [
  42,
  EmptyInnerModule,
  InnerModule2,
  Z,
  y
];

function testConvert(m) {
  return m;
}

function SomeFunctor(X) {
  var ww = X[/* y */4];
  return /* module */[/* ww */ww];
}

function someFunctorAsFunction(x) {
  var ww = x[/* y */4];
  return /* module */[/* ww */ww];
}

export {
  M ,
  firstClassModule ,
  testConvert ,
  SomeFunctor ,
  someFunctorAsFunction ,
  
}
/* No side effect */