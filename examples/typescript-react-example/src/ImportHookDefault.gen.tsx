/* TypeScript file generated by genType. */
/* eslint-disable import/first */


import {default as makeNotChecked} from './hookExample';

// In case of type error, check the type of 'make' in 'ImportHookDefault.re' and './hookExample'.
export const makeTypeChecked: React.FC<{
  readonly person: person; 
  readonly children: JSX.Element; 
  readonly renderMe: React.FC<{
    readonly randomString: string
  }>
}> = makeNotChecked;

// Export 'make' early to allow circular import from the '.bs.js' file.
export const make: unknown = function hookExample(Arg1: any) {
  const result = makeTypeChecked({person:{name:Arg1.person[0], age:Arg1.person[1]}, children:Arg1.children, renderMe:Arg1.renderMe});
  return result
} as React.FC<{
  readonly person: person; 
  readonly children: JSX.Element; 
  readonly renderMe: React.FC<{
    readonly randomString: string
  }>
}>;

// tslint:disable-next-line:interface-over-type-literal
export type person = { readonly name: string; readonly age: number };
