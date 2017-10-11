import { assert, expect } from "chai";
import * as mocha from "mocha";

// If moduleResolution is node
import { createInstance, SampleObjects, App, Business, Product } from "threadily.sampleobjects.javascript";

describe('App Example', () => {
    let module: SampleObjects;

    beforeEach(() => {
        module = createInstance({
            onRuntimeInitialized: () => {
            },
            locateFile: (filename) => {
                console.log(filename);                
                if (filename === 'threadily.sampleobjects.javascript.js.mem') {
                    return "node_modules/threadily.sampleObjects.javascript/ship/threadily.sampleobjects.javascript.js.mem"
                }
            },
        });
    });

    it('Factory creates App, App creates Business, Business reads products', (done: MochaDone) => {
        let createProduct = (newBusiness: Business, productName: string, onDone: () => void) => {
            let handle = newBusiness.isCreateProductPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        newBusiness.isCreateProductPending.unsubscribe(handle);
                        // console.log("Reading products complete");
                        onDone();
                    }
                }
            }));
            // console.log("Creating product");
            newBusiness.createProductAsync(productName);
        }
        let readProducts = (newBusiness: Business, queryString: string, onDone: () => void) => {
            let handle = newBusiness.isProductsPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        newBusiness.isProductsPending.unsubscribe(handle);
                        // console.log("Reading products complete");
                        onDone();
                    }
                }
            }));
            // console.log("Reading products");
            newBusiness.readProductsAsync(0, 20, queryString);
        }
        let createBusiness = (app: App, businessName: string, onDone: () => void) => {
            let handle = app.isCreateBusinessPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        app.isCreateBusinessPending.unsubscribe(handle);
                        // console.log("Creating Businesss complete");
                        onDone();
                    }
                }
            }));
            // console.log("Creating Business");
            app.createBusinessAsync(businessName);
        }
        let readBusinesss = (app: App, queryString: string, onDone: () => void) => {
            let handle = app.isBusinessesPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        app.isBusinessesPending.unsubscribe(handle);
                        // console.log("Reading businesses complete");
                        onDone();
                    }
                }
            }));
            // console.log("Reading businesses");
            app.readBusinessesAsync(0, 20, queryString);
        }

        let app = module.AppFactory.getInstance().create();
        readBusinesss(app, "", () => {
            assert.equal(app.businesses.size(), 0, "Expected 0 businesses at first");
            createBusiness(app, "Business", () => {
                let business = app.createdBusiness.get();
                readBusinesss(app, "", () => {
                    assert.equal(app.businesses.size(), 1, "Expected 1 businesses after create");
                    readProducts(business, "", () => {
                        assert.equal(business.products.size(), 0, "Expected 1 products after create");
                        createProduct(business, "Product 1", () => {
                            readProducts(business, "", () => {
                                assert.equal(business.products.size(), 1, "Expected 1 products after create");
                                createProduct(business, "Product 2", () => {
                                    readProducts(business, "", () => {
                                        assert.equal(business.products.size(), 2, "Expected 1 products after create");
                                        done();
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
    });

    it('Create product on business out of scope', (done: MochaDone) => {
        let createProduct = (newBusiness, productName, onDone) => {
            let handle = newBusiness.isCreateProductPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        newBusiness.isCreateProductPending.unsubscribe(handle);
                        // console.log("Create product complete");
                        onDone();
                    }
                }
            }));
            // console.log("Creating product");
            newBusiness.createProductAsync(productName);
        }
        let createBusiness = (app, businessName, onDone) => {
            let handle = app.isCreateBusinessPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        app.isCreateBusinessPending.unsubscribe(handle);
                        app.createdBusiness.clone();
                        // console.log("Creating Businesss complete");
                        onDone();
                    }
                }
            }));
            // console.log("Creating Business");
            app.createBusinessAsync(businessName);
        }

        let app = module.AppFactory.getInstance().create();
        let createdBusinessHandle = app.createdBusiness.subscribe(new module.ISubscribeHandleBusinessCallback.implement({
            onChange(newBusiness) {
                if (newBusiness != null) {
                    app.createdBusiness.unsubscribe(createdBusinessHandle);
                    setTimeout(() => {
                        assert.throws(() => {
                            newBusiness.createProductAsync("Product");
                        }, "Cannot pass deleted object as a pointer of type Business*");
                        done();
                    }, 1);
                }
            }
        }));
        createBusiness(app, "Business", () => { });
    });

    it('Clone subscription', (done: MochaDone) => {
        let createProduct = (newBusiness, productName, onDone) => {
            let handle = newBusiness.isCreateProductPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        newBusiness.isCreateProductPending.unsubscribe(handle);
                        // console.log("Reading products complete");
                        onDone();
                    }
                }
            }));
            // console.log("Creating product");
            newBusiness.createProductAsync(productName);
        }
        let createBusiness = (app, businessName, onDone) => {
            let handle = app.isCreateBusinessPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        app.isCreateBusinessPending.unsubscribe(handle);
                        // console.log("Creating Businesss complete");
                        onDone();
                    }
                }
            }));
            // console.log("Creating Business");
            app.createBusinessAsync(businessName);
        }

        let app = module.AppFactory.getInstance().create();
        app.createdBusiness.subscribe(new module.ISubscribeHandleBusinessCallback.implement({
            onChange(newBusiness) {
                if (newBusiness != null) {
                    newBusiness = newBusiness.clone();
                    setTimeout(() => {
                        createProduct(newBusiness, "Product", () => {
                            newBusiness.delete();
                            assert.throws(() => {
                                newBusiness.createProductAsync("Product after deleted");
                            }, "Cannot pass deleted object as a pointer of type Business*");
                            done();
                        });
                    }, 1);
                }
            }
        }));
        createBusiness(app, "Business", () => {
        });
    });
});
