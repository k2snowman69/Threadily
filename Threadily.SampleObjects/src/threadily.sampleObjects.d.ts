import * as threadily from "threadily";

declare module 'threadily.sampleObjects' {
    interface Product {
        name: threadily.Observable<string>;
    }
    interface Business {
		name: threadily.Observable<string>;
        isCreateProductPending: threadily.Observable<boolean>;
        createdProduct: threadily.Observable<Product>;
        isProductsPending: threadily.Observable<boolean>;
		products: threadily.ObservableVector<Product>;
		readProductsAsync(index: number, pageSize: number, queryString: string): void;
		createProductAsync(name: string): void;
    }
    interface App {
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
    interface AppFactory {
        getInstance(): AppFactory;
        create(): App;
    }
    interface PrimativesThreadObject extends threadily.ThreadObject<threadily.ThreadObjectId> {

    }
    interface PrimativesThreadObjectManager extends threadily.ThreadObjectManager<PrimativesThreadObject>{

    }
    interface ThreadablesThreadObject extends threadily.ThreadObject<threadily.ThreadObjectId> {

    }
    interface ThreadablesThreadObjectManager extends threadily.ThreadObjectManager<ThreadablesThreadObject>{

    }
    interface EmptyThreadObject extends threadily.ThreadObject<threadily.ThreadObjectId> {

    }
    interface EmptyThreadObjectManager extends threadily.ThreadObjectManager<EmptyThreadObject>{

    }
    interface SampleObjects extends threadily.Threadily {
        AppFactory: AppFactory;
        ISubscribeHandleBusinessCallback: threadily.ObservableCallback<Business>;
        ISubscribeHandleEmptyThreadObjectCallback: threadily.ObservableCallback<EmptyThreadObject>;
        ISubscribeHandleEmptyThreadObjectVectorCallback: threadily.ObservableVectorCallback<EmptyThreadObject>;
        PrimativesThreadObjectManager: PrimativesThreadObjectManager;
        ThreadablesThreadObjectManager: ThreadablesThreadObjectManager;
        EmptyThreadObjectManager: EmptyThreadObjectManager;
    }
}