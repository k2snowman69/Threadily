// When exporting Threadily to javascript using Emscripten, this definition file defines all the outputted types from Threadily

declare module "threadily" {
    // Observable.cpp
    interface ISubscribeHandle {
    }
    interface ObservableCallback<T> {
        onChange(T): void;
        implement: any;
    }
    interface Observable<T> {
        get(): T;
        set(newValue: T): void;
        subscribe(callback: ObservableCallback<T>): ISubscribeHandle;
        unsubscribe(handle: ISubscribeHandle): void;
    }
    enum ObservableActionType {
        Insert = 0,
        Erase = 1,
        Set = 2,
    }
    interface ObservableVectorCallback<T> {
        onChange(newValue: T): void;
        implement: any;
    }
    interface ObservableVector<T> {
        size(): number;
        at(index: number): T;
        insert(index: number, value: T): void;
        set(index: number, value: T): void;
        erase(index: number): void;

        subscribe(callback: ObservableVectorCallback<T>, index: number, action: ObservableActionType): ISubscribeHandle;
        unsubscribe(handle: ISubscribeHandle): void;
    }
    // ThreadManager.cpp
    interface VectorUnsignedInt {
        new(): VectorUnsignedInt;
        push_back(value: number): void;
    }
    interface IThreadManager {
        clear(): void;
    }
    interface ThreadManager extends IThreadManager {
        new(): ThreadManager;
        // TODO: Replace any with an actual definition
        getOrCreateThread(threadId: number, notifiesThreadIds: VectorUnsignedInt, runner: any): any;
    }
    // ThreadObject.cpp
    interface IThreadObject {
        getThreadId: () => number;
        getInstanceId: () => IThreadObjectId;
        getReference: () => IThreadObject;
    }
    interface ThreadObject<T extends IThreadObjectId> extends IThreadObject {
    }
    interface ThreadObject_ThreadObjectId extends ThreadObject<ThreadObjectId> {
    }
    // ThreadObjectId.cpp
    interface IThreadObjectId {
    }
    interface ThreadObjectId {
        instanceId: number;
    }
    // ThreadObjectManager.cpp
    interface IThreadObjectManager {
        getThreadManager(): IThreadManager;
    }
    interface ThreadObjectManager<T> extends IThreadObjectManager {
        new(threadManager: ThreadManager);
        getOrCreateObject(threadId: number, id: IThreadObjectId): T;
    }
    // ThreadQueueItem.cpp - Ignored as it's used internally
    // ThreadQueueItemHandler.cpp - Ignored as it's used internally
    // ThreadQueueItemManager.cpp - Ignored as it's used internally
    interface Threadily {
        ThreadManager: ThreadManager;
        VectorUnsignedInt: VectorUnsignedInt;
        ISubscribeHandleBoolCallback: ObservableCallback<boolean>;
        ISubscribeHandleWStringCallback: ObservableCallback<string>;
        ISubscribeHandleWStringVectorCallback: ObservableVectorCallback<string>;
    }
}