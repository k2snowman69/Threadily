import { assert, expect } from "chai";
import * as mocha from "mocha";

// If moduleResolution is node
import { createInstance, SampleObjects, App, Business, Product } from "threadily.sampleobjects"

describe('ThreadablesThreadObjectUnitTest', () => {
    let module: SampleObjects;

    beforeEach(() => {
        module = createInstance({
            onRuntimeInitialized: () => {
            },
            locateFile: (filename) => {
                if (filename === 'threadily.sampleObjects.js.mem') {
                    return "node_modules/threadily.sampleObjects/ship/threadily.sampleObjects.js.mem"
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
                assert.ok(newObject.id == emptyObject.id, "Should get a notification for the new name");
                done();
            }
        }));

        assert.ok(obj_UI.emptyObject.get() == null, "Name should be empty before doing anything");
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
                assert.equal(newObject.getThreadId(), 0);
                assert.equal(index, 0);
                done();
            }
        }));

        assert.ok(obj_UI.emptyObjectArray.size() == "", "Array should be empty before doing anything");
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
                assert.equal(newObject.getThreadId(), 0);
                assert.equal(index, 2);
                done();
            }
        }));

        assert.ok(obj_UI.emptyObjectArray.size() == "", "Array should be empty before doing anything");
        obj_Service.emptyObjectArray.insert(2, emptyObject);
    })
})
