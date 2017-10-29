// When exporting Threadily to javascript using Emscripten, this definition file defines all the outputted types from Threadily

declare module "threadily" {
    // Observable.cpp
    export interface ISubscribeHandle {
    }
    export interface ObservableCallback<T> {
        onChange(T): void;
        implement: any;
    }
    export interface Observable<T> {
        get(): T;
        set(newValue: T): void;
        subscribe(callback: ObservableCallback<T>): ISubscribeHandle;
        unsubscribe(handle: ISubscribeHandle): void;
    }
    export enum ObservableActionType {
        Insert = 0,
        Erase = 1,
        Set = 2,
    }
    export interface ObservableVectorCallback<T> {
        onChange(newValue: T): void;
        implement: any;
    }
    export interface ObservableVector<T> {
        size(): number;
        at(index: number): T;
        insert(index: number, value: T): void;
        set(index: number, value: T): void;
        erase(index: number): void;

        subscribe(callback: ObservableVectorCallback<T>, index: number, action: ObservableActionType): ISubscribeHandle;
        unsubscribe(handle: ISubscribeHandle): void;
    }
    // ThreadManager.cpp
    export interface VectorUnsignedInt {
        new(): VectorUnsignedInt;
        push_back(value: number): void;
    }
    export interface IThreadManager {
        clear(): void;
    }
    export interface ThreadManager extends IThreadManager {
        new(): ThreadManager;
        // TODO: Replace any with an actual definition
        getOrCreateThread(threadId: number, notifiesThreadIds: VectorUnsignedInt, runner: any): any;
    }
    // ThreadObject.cpp
    export interface IThreadObject {
        getThreadId: () => number;
        getInstanceId: () => IThreadObjectId;
        clone<T>(): T;
        delete: () => void;
        isDeleted: () => boolean;
    }
    export interface ThreadObject<T extends IThreadObjectId> extends IThreadObject {
        getId: () => T;
    }
    export interface ThreadObject_ThreadObjectId extends ThreadObject<ThreadObjectId> {
    }
    // ThreadObjectId.cpp
    export interface IThreadObjectId {
    }
    export interface ThreadObjectId {
        instanceId: number;
    }
    // ThreadObjectManager.cpp
    export interface IThreadObjectManager {
        getThreadManager(): IThreadManager;
    }
    export interface ThreadObjectManager<T> extends IThreadObjectManager {
        new(threadManager: ThreadManager);
        getOrCreateObject(threadId: number, id: IThreadObjectId): T;
    }
    // ThreadQueueItem.cpp - Ignored as it's used internally
    // ThreadQueueItemHandler.cpp - Ignored as it's used internally
    // ThreadQueueItemManager.cpp - Ignored as it's used internally
    export interface Threadily {
        ThreadManager: ThreadManager;
        VectorUnsignedInt: VectorUnsignedInt;
        ISubscribeHandleBoolCallback: ObservableCallback<boolean>;
        ISubscribeHandleWStringCallback: ObservableCallback<string>;
        ISubscribeHandleWStringVectorCallback: ObservableVectorCallback<string>;
    }
}