declare module 'threadily.sampleObjects' {
    // BEGIN TODO - rickyp - Move these to Threadily.d.ts
    interface Implement {
        new(any): any;
    }
    interface ISubscribeHandleCallback<T> {
        implement: Implement;
    }
    interface ISubscribeHandleBoolCallback extends ISubscribeHandleCallback<boolean> {
    }
    interface Observable<T> {
        get(): T;
        subscribe(any): any;
        unsubscribe(any): void;
    }
    interface ObservableArray<T> {
        size(): number;
    }
    // END TODO
    interface ISubscribeHandleBusinessCallback extends ISubscribeHandleCallback<Business>{
    }
    interface Product {
        name: Observable<string>;
    }
    interface Business {
		name: Observable<string>;
        isCreateProductPending: Observable<boolean>;
        createdProduct: Observable<Product>;
        isProductsPending: Observable<boolean>;
		products: ObservableArray<Product>;
		readProductsAsync(): void;
		createProductAsync(): void;
    }
    interface App {
        isBusinessesPending: Observable<boolean>;
        businesses: ObservableArray<Business>;
        isCreateBusinessPending: Observable<boolean>;
        createdBusiness: Observable<Business>;
        createBusinessAsync(): void;
        readBusinessesAsync(): void;
        throwTest1(): void;
        throwTest2(): void;
        throwTest3(): void;
    }
    interface AppFactory {
        getInstance(): AppFactory;
        create(): App;
    }
    interface SampleObjects {
        AppFactory: AppFactory;
        ISubscribeHandleBoolCallback: ISubscribeHandleBoolCallback;
        ISubscribeHandleBusinessCallback: ISubscribeHandleBusinessCallback;
    }
    export function createInstance(emscriptenArgs: any): SampleObjects;
}