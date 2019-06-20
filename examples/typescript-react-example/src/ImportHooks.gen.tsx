/* TypeScript file generated by genType. */
/* eslint-disable import/first */


import {make as makeNotChecked} from './hookExample';

import {foo as fooNotChecked} from './hookExample';

// In case of type error, check the type of 'make' in 'ImportHooks.re' and './hookExample'.
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

// In case of type error, check the type of 'foo' in 'ImportHooks.re' and './hookExample'.
export const fooTypeChecked: (_1:{ readonly person: person }) => string = fooNotChecked;

// Export 'foo' early to allow circular import from the '.bs.js' file.
export const foo: unknown = function (Argperson: any) {
  const result = fooTypeChecked({person:{name:Argperson[0], age:Argperson[1]}});
  return result
} as (_1:{ readonly person: person }) => string;

// tslint:disable-next-line:interface-over-type-literal
export type person = { readonly name: string; readonly age: number };
