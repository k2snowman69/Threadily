import * as threadily from "threadily";

declare module 'sample-objects-cpp' {
    export interface Product {
        name: threadily.Observable<string>;
    }
    export interface Business {
        name: threadily.Observable<string>;
        isCreateProductPending: threadily.Observable<boolean>;
        createdProduct: threadily.Observable<Product>;
        isProductsPending: threadily.Observable<boolean>;
        products: threadily.ObservableVector<Product>;
        readProductsAsync(index: number, pageSize: number, queryString: string): void;
        createProductAsync(name: string): void;
    }
    export interface App {
        isBusinessesPending: threadily.Observable<boolean>;
        businesses: threadily.ObservableVector<Business>;
        isCreateBusinessPending: threadily.Observable<boolean>;
        createdBusiness: threadily.Observable<Business>;
        createBusinessAsync(name: string): void;
        readBusinessesAsync(index: number, pageSize: number, queryString: string): void;
        throwTest1(): void;
        throwTest2(): void;
        throwTest3(): void;
    }
    export interface AppFactory {
        getInstance(): AppFactory;
        create(): App;
    }
    export interface PrimativesThreadObject extends threadily.ThreadObject<threadily.ThreadObjectId> {

    }
    export interface PrimativesThreadObjectManager extends threadily.ThreadObjectManager<PrimativesThreadObject> {

    }
    export interface ThreadablesThreadObject extends threadily.ThreadObject<threadily.ThreadObjectId> {

    }
    export interface ThreadablesThreadObjectManager extends threadily.ThreadObjectManager<ThreadablesThreadObject> {

    }
    export interface EmptyThreadObject extends threadily.ThreadObject<threadily.ThreadObjectId> {

    }
    export interface EmptyThreadObjectManager extends threadily.ThreadObjectManager<EmptyThreadObject> {

    }
    export interface SampleObjects extends threadily.Threadily {
        AppFactory: AppFactory;
        ISubscribeHandleBusinessCallback: threadily.ObservableCallback<Business>;
        ISubscribeHandleEmptyThreadObjectCallback: threadily.ObservableCallback<EmptyThreadObject>;
        ISubscribeHandleEmptyThreadObjectVectorCallback: threadily.ObservableVectorCallback<EmptyThreadObject>;
        PrimativesThreadObjectManager: PrimativesThreadObjectManager;
        ThreadablesThreadObjectManager: ThreadablesThreadObjectManager;
        EmptyThreadObjectManager: EmptyThreadObjectManager;
    }
}