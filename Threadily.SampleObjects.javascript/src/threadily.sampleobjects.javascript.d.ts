import { SampleObjects } from "threadily.sampleobjects";
export * from "threadily.sampleobjects";

declare module 'threadily.sampleObjects.javascript' {
    export function createInstance(emscriptenArgs: any): SampleObjects;
}