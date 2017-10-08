import { assert, expect } from "chai";
import * as mocha from "mocha";

// If moduleResolution is node
import { createInstance, SampleObjects, App, Business, Product } from "threadily.sampleobjects"

describe('PrimativesThreadObjectUnitTest', () => {
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

    it('SetName Service -> UI notification', (done: MochaDone) => {
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

        let PrimativesThreadObjectManager = new module.PrimativesThreadObjectManager(threadManager);

        let obj_Service = PrimativesThreadObjectManager.getOrCreateObject(2, { instanceId: 0 });
        let obj_UI = PrimativesThreadObjectManager.getOrCreateObject(0, { instanceId: 0 });

        let newName = "A name";

        let handler = obj_UI.name.subscribe(new module.ISubscribeHandleWStringCallback.implement({
            onChange(newString) {
                assert.ok(newString === newName, "Should get a notification for the new name");
                done();
            }
        }));

        assert.ok(obj_UI.name.get() === "", "Name should be empty before doing anything");
        obj_Service.name.set(newName);
    });

    it('StringArray - Insert - Position 0', (done: MochaDone) => {
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

        let PrimativesThreadObjectManager = new module.PrimativesThreadObjectManager(threadManager);

        let obj_Service = PrimativesThreadObjectManager.getOrCreateObject(2, { instanceId: 0 });
        let obj_UI = PrimativesThreadObjectManager.getOrCreateObject(0, { instanceId: 0 });

        let handler = obj_UI.stringArray.subscribe(new module.ISubscribeHandleWStringVectorCallback.implement({
            onChange(newInt, index, action) {
                assert.equal(index, 0);
                done();
            }
        }));

        assert.ok(obj_UI.stringArray.size() === 0, "Array should be empty before doing anything");
        obj_Service.stringArray.insert(0, 'Item 0');
    });

    it('StringArray - Insert - Out of order', (done: MochaDone) => {
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

        let PrimativesThreadObjectManager = new module.PrimativesThreadObjectManager(threadManager);

        let obj_Service = PrimativesThreadObjectManager.getOrCreateObject(2, { instanceId: 0 });
        let obj_UI = PrimativesThreadObjectManager.getOrCreateObject(0, { instanceId: 0 });

        let handler = obj_UI.stringArray.subscribe(new module.ISubscribeHandleWStringVectorCallback.implement({
            onChange(newInt, index, action) {
                assert.equal(index, 2);
                done();
            }
        }));

        assert.ok(obj_UI.stringArray.size() === 0, "Array should be empty before doing anything");
        obj_Service.stringArray.insert(2, 'Item 2');
    });
});
