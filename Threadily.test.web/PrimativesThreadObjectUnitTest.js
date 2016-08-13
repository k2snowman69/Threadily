var assert = require('assert');
var module = require('./Threadily.SampleObjects');

describe('PrimativesThreadObjectUnitTest', function () {

    it('SetName Service -> UI notification', function (done) {
        var threadManager = new module.ThreadManager();
        // UI Thread
        threadManager.getOrCreateThread(0, new module.VectorUnsignedInt(), null);
        // App Thread
        var notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(0);
        threadManager.getOrCreateThread(1, notifiedThreads, null);
        // Service Thread
        notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(1);
        threadManager.getOrCreateThread(2, notifiedThreads, null);

        var PrimativesThreadObjectManager = new module.PrimativesThreadObjectManager(threadManager);

        var obj_Service = PrimativesThreadObjectManager.getOrCreateObject(2, 0);
        var obj_UI = PrimativesThreadObjectManager.getOrCreateObject(0, 0);

        var newName = "A name";

        var handler = obj_UI.name.subscribe(new module.ISubscribeHandleWStringCallback.implement({
            onChange(newString) {
                assert.ok(newString == newName, "Should get a notification for the new name");
                done();
            }
        }));

        assert.ok(obj_UI.name.get() == "", "Name should be empty before doing anything");
        obj_Service.name.set(newName);
    })

    it('StringArray - Insert - Position 0', function (done) {
        var threadManager = new module.ThreadManager();
        // UI Thread
        threadManager.getOrCreateThread(0, new module.VectorUnsignedInt(), null);
        // App Thread
        var notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(0);
        threadManager.getOrCreateThread(1, notifiedThreads, null);
        // Service Thread
        notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(1);
        threadManager.getOrCreateThread(2, notifiedThreads, null);

        var PrimativesThreadObjectManager = new module.PrimativesThreadObjectManager(threadManager);

        var obj_Service = PrimativesThreadObjectManager.getOrCreateObject(2, 0);
        var obj_UI = PrimativesThreadObjectManager.getOrCreateObject(0, 0);

        var handler = obj_UI.stringArray.subscribe(new module.ISubscribeHandleWStringVectorCallback.implement({
            onChange(newInt, index, action) {
                assert.equal(index, 0);
                done();
            }
        }));

        assert.ok(obj_UI.stringArray.size() == "", "Array should be empty before doing anything");
        obj_Service.stringArray.insert(0, 'Item 0');
    })

    it('StringArray - Insert - Out of order', function (done) {
        var threadManager = new module.ThreadManager();
        // UI Thread
        threadManager.getOrCreateThread(0, new module.VectorUnsignedInt(), null);
        // App Thread
        var notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(0);
        threadManager.getOrCreateThread(1, notifiedThreads, null);
        // Service Thread
        notifiedThreads = new module.VectorUnsignedInt();
        notifiedThreads.push_back(1);
        threadManager.getOrCreateThread(2, notifiedThreads, null);

        var PrimativesThreadObjectManager = new module.PrimativesThreadObjectManager(threadManager);

        var obj_Service = PrimativesThreadObjectManager.getOrCreateObject(2, 0);
        var obj_UI = PrimativesThreadObjectManager.getOrCreateObject(0, 0);

        var handler = obj_UI.stringArray.subscribe(new module.ISubscribeHandleWStringVectorCallback.implement({
            onChange(newInt, index, action) {
                assert.equal(index, 2);
                done();
            }
        }));

        assert.ok(obj_UI.stringArray.size() == "", "Array should be empty before doing anything");
        obj_Service.stringArray.insert(2, 'Item 2');
    })
})
