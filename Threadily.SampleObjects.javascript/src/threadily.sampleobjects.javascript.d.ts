import { SampleObjects } from "threadily.sampleObjects";
export * from "threadily.sampleObjects";

declare module 'threadily.sampleObjects.javascript' {
    export function createInstance(emscriptenArgs: any): SampleObjects;
}