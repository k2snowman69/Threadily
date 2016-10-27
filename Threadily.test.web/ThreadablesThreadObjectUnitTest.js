var assert = require('assert');
var module = require('./Threadily.SampleObjects');

describe('ThreadablesThreadObjectUnitTest', function () {

    it('Set emptyObject - Service -> UI notification', function (done) {
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

        var threadablesThreadObjectManager = new module.ThreadablesThreadObjectManager(threadManager);
        var emptyObjectManager = new module.EmptyThreadObjectManager(threadManager);

        var obj_Service = threadablesThreadObjectManager.getOrCreateObject(2, 0);
        var obj_UI = threadablesThreadObjectManager.getOrCreateObject(0, 0);

        var emptyObject = emptyObjectManager.getOrCreateObject(2, 0);

        var handler = obj_UI.emptyObject.subscribe(new module.ISubscribeHandleEmptyThreadObjectCallback.implement({
            onChange(newObject) {
                assert.ok(newObject.id == emptyObject.id, "Should get a notification for the new name");
                done();
            }
        }));

        assert.ok(obj_UI.emptyObject.get() == null, "Name should be empty before doing anything");
        obj_Service.emptyObject.set(emptyObject);
    })

    it('emptyObjectArray - Insert - Position 0', function (done) {
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

        var threadablesThreadObjectManager = new module.ThreadablesThreadObjectManager(threadManager);
        var emptyObjectManager = new module.EmptyThreadObjectManager(threadManager);

        var obj_Service = threadablesThreadObjectManager.getOrCreateObject(2, 0);
        var obj_UI = threadablesThreadObjectManager.getOrCreateObject(0, 0);

        var emptyObject = emptyObjectManager.getOrCreateObject(2, 0);

        var handler = obj_UI.emptyObjectArray.subscribe(new module.ISubscribeHandleEmptyThreadObjectVectorCallback.implement({
            onChange(newObject, index, action) {
                assert.equal(newObject.getThreadId(), 0);
                assert.equal(index, 0);
                done();
            }
        }));

        assert.ok(obj_UI.emptyObjectArray.size() == "", "Array should be empty before doing anything");
        obj_Service.emptyObjectArray.insert(0, emptyObject);
    })

    it('emptyObjectArray - Insert - Position 2', function (done) {
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

        var threadablesThreadObjectManager = new module.ThreadablesThreadObjectManager(threadManager);
        var emptyObjectManager = new module.EmptyThreadObjectManager(threadManager);

        var obj_Service = threadablesThreadObjectManager.getOrCreateObject(2, 0);
        var obj_UI = threadablesThreadObjectManager.getOrCreateObject(0, 0);

        var emptyObject = emptyObjectManager.getOrCreateObject(2, 0);

        var handler = obj_UI.emptyObjectArray.subscribe(new module.ISubscribeHandleEmptyThreadObjectVectorCallback.implement({
            onChange(newObject, index, action) {
                assert.equal(newObject.getThreadId(), 0);
                assert.equal(index, 2);
                done();
            }
        }));

        assert.ok(obj_UI.emptyObjectArray.size() == "", "Array should be empty before doing anything");
        obj_Service.emptyObjectArray.insert(2, emptyObject);
    })

    it('Keep object alive', function (done) {
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

        var threadablesThreadObjectManager = new module.ThreadablesThreadObjectManager(threadManager);
        var emptyObjectManager = new module.EmptyThreadObjectManager(threadManager);

        var obj_Service = threadablesThreadObjectManager.getOrCreateObject(2, 0);
        var obj_UI = threadablesThreadObjectManager.getOrCreateObject(0, 0);

        var emptyObject = emptyObjectManager.getOrCreateObject(2, 0);

        var handler = obj_UI.emptyObjectArray.subscribe(new module.ISubscribeHandleEmptyThreadObjectVectorCallback.implement({
            onChange(newObject, index, action) {
                var newObject = module.ThreadObject.getReference(newObject);
                setTimeout(function () {
                    assert.equal(newObject.getThreadId(), 0);
                    newObject.delete();
                    setTimeout(function () {
                        try {
                            newObject.getThreadId();
                            done("Should have fired an exeption due to the object being disposed");
                        }
                        catch (e) {
                            done();
                        }
                    });
                });
            }
        }));

        assert.ok(obj_UI.emptyObjectArray.size() == "", "Array should be empty before doing anything");
        obj_Service.emptyObjectArray.insert(2, emptyObject);
    })
})
