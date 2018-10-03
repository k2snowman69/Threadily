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

    it('SetName Service -> UI notification', (done) => {
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
                expect(newString).toBe(newName);
                done();
            }
        }));

        // Name should be empty before doing anything
        expect(obj_UI.name.get()).toBe("");
        obj_Service.name.set(newName);
    });

    it('StringArray - Insert - Position 0', (done) => {
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
                expect(index).toBe(0);
                done();
            }
        }));

        // Array should be empty before doing anything
        expect(obj_UI.stringArray.size()).toBe(0);
        obj_Service.stringArray.insert(0, 'Item 0');
    });

    it('StringArray - Insert - Out of order', (done) => {
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
                expect(index).toBe(2);
                done();
            }
        }));

        // Array should be empty before doing anything
        expect(obj_UI.stringArray.size()).toBe(0);
        obj_Service.stringArray.insert(2, 'Item 2');
    });
});