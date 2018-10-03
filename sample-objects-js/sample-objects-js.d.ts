import { SampleObjects } from "sample-objects-cpp";
export * from "sample-objects-cpp";

declare module "sample-objects-js" {
    export default function Module(emscriptenArgs: any): SampleObjects;
}