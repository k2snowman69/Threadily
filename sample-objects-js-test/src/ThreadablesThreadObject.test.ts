import Module, { SampleObjects, App, Business, Product } from "sample-objects-js";

describe('PrimativesThreadObjectUnitTest', () => {
    let module: SampleObjects;

    beforeEach(() => {
        module = Module({
            onRuntimeInitialized: () => {
            },
            locateFile: (filename) => {
                if (filename === 'sample-objects-js.js.mem') {
                    return "node_modules/sample-objects-js/ship/sample-objects-js.js.mem"
                }
            },
        });
    });

    it('Set emptyObject - Service -> UI notification', (done) => {
        let threadManager = new module.ThreadManager();
        // UI Thread
        threadManager.getOrCreateThread(0, new module.VectorUnsignedInt(), null);
        // App Thread
        let notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(0);
        threadManager.getOrCreateThread(1, notifiedThreads, null);
        // Service Thread
        notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(1);
        threadManager.getOrCreateThread(2, notifiedThreads, null);

        let threadablesThreadObjectManager = new module.ThreadablesThreadObjectManager(threadManager);
        let emptyObjectManager = new module.EmptyThreadObjectManager(threadManager);

        let obj_Service = threadablesThreadObjectManager.getOrCreateObject(2, { instanceId: 0 });
        let obj_UI = threadablesThreadObjectManager.getOrCreateObject(0, { instanceId: 0 });

        let emptyObject = emptyObjectManager.getOrCreateObject(2, { instanceId: 0 });

        let handler = obj_UI.emptyObject.subscribe(new module.ISubscribeHandleEmptyThreadObjectCallback.implement({
            onChange(newObject) {
                // Should get a notification for the new name
                expect(newObject.id).toBe(emptyObject.id);
                done();
            }
        }));

        // Name should be empty before doing anything
        expect(obj_UI.emptyObject.get()).toBe(null);
        obj_Service.emptyObject.set(emptyObject);
    })

    it('emptyObjectArray - Insert - Position 0', (done) => {
        let threadManager = new module.ThreadManager();
        // UI Thread
        threadManager.getOrCreateThread(0, new module.VectorUnsignedInt(), null);
        // App Thread
        let notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(0);
        threadManager.getOrCreateThread(1, notifiedThreads, null);
        // Service Thread
        notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(1);
        threadManager.getOrCreateThread(2, notifiedThreads, null);

        let threadablesThreadObjectManager = new module.ThreadablesThreadObjectManager(threadManager);
        let emptyObjectManager = new module.EmptyThreadObjectManager(threadManager);

        let obj_Service = threadablesThreadObjectManager.getOrCreateObject(2, { instanceId: 0 });
        let obj_UI = threadablesThreadObjectManager.getOrCreateObject(0, { instanceId: 0 });

        let emptyObject = emptyObjectManager.getOrCreateObject(2, { instanceId: 0 });

        let handler = obj_UI.emptyObjectArray.subscribe(new module.ISubscribeHandleEmptyThreadObjectVectorCallback.implement({
            onChange(newObject, index, action) {
                expect(newObject.getThreadId()).toBe(0);
                expect(index).toBe(0);
                done();
            }
        }));

        // Array should be empty before doing anything
        expect(obj_UI.emptyObjectArray.size()).toBe(0);
        obj_Service.emptyObjectArray.insert(0, emptyObject);
    })

    it('emptyObjectArray - Insert - Position 2', (done) => {
        let threadManager = new module.ThreadManager();
        // UI Thread
        threadManager.getOrCreateThread(0, new module.VectorUnsignedInt(), null);
        // App Thread
        let notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(0);
        threadManager.getOrCreateThread(1, notifiedThreads, null);
        // Service Thread
        notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(1);
        threadManager.getOrCreateThread(2, notifiedThreads, null);

        let threadablesThreadObjectManager = new module.ThreadablesThreadObjectManager(threadManager);
        let emptyObjectManager = new module.EmptyThreadObjectManager(threadManager);

        let obj_Service = threadablesThreadObjectManager.getOrCreateObject(2, { instanceId: 0 });
        let obj_UI = threadablesThreadObjectManager.getOrCreateObject(0, { instanceId: 0 });

        let emptyObject = emptyObjectManager.getOrCreateObject(2, { instanceId: 0 });

        let handler = obj_UI.emptyObjectArray.subscribe(new module.ISubscribeHandleEmptyThreadObjectVectorCallback.implement({
            onChange(newObject, index, action) {
                expect(newObject.getThreadId()).toBe(0);
                expect(index).toBe(2);
                done();
            }
        }));

        // Array should be empty before doing anything
        expect(obj_UI.emptyObjectArray.size()).toBe(0);
        obj_Service.emptyObjectArray.insert(2, emptyObject);
    })
})