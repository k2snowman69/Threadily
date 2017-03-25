var assert = require('assert');
var module = require('./Threadily.SampleObjects');

describe('App Example', function () {

    it('Factory creates App, App creates Business, Business reads products', function (done) {
        var handle = null;

        var createProduct = function (newBusiness, productName, onDone) {
            handle = newBusiness.isCreateProductPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        newBusiness.isCreateProductPending.unsubscribe(handle);
                        console.log("Reading products complete");
                        onDone();
                    }
                }
            }));
            console.log("Creating product");
            newBusiness.createProductAsync(productName);
        }
        var readProducts = function (newBusiness, queryString, onDone) {
            handle = newBusiness.isProductsPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        newBusiness.isProductsPending.unsubscribe(handle);
                        console.log("Reading products complete");
                        onDone();
                    }
                }
            }));
            console.log("Reading products");
            newBusiness.readProductsAsync(0, 20, queryString);
        }
        var createBusiness = function (app, businessName, onDone) {
            handle = app.isCreateBusinessPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        app.isCreateBusinessPending.unsubscribe(handle);
                        console.log("Creating Businesss complete");
                        onDone();
                    }
                }
            }));
            console.log("Creating Business");
            app.createBusinessAsync(businessName);
        }
        var readBusinesss = function (app, queryString, onDone) {
            handle = app.isBusinessesPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        app.isBusinessesPending.unsubscribe(handle);
                        console.log("Reading businesses complete");
                        onDone();
                    }
                }
            }));
            console.log("Reading businesses");
            app.readBusinessesAsync(0, 20, queryString);
        }

        var app = new module.AppFactory.getInstance().create();
        readBusinesss(app, "", function () {
            assert.equal(app.businesses.size(), 0, "Expected 0 businesses at first");
            createBusiness(app, "Business", function () {
                var business = app.createdBusiness.get();
                readBusinesss(app, "", function () {
                    assert.equal(app.businesses.size(), 1, "Expected 1 businesses after create");
                    readProducts(business, "", function () {
                        assert.equal(business.products.size(), 0, "Expected 1 products after create");
                        createProduct(business, "Product 1", function () {
                            readProducts(business, "", function () {
                                assert.equal(business.products.size(), 1, "Expected 1 products after create");
                                createProduct(business, "Product 2", function () {
                                    readProducts(business, "", function () {
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
    it('throw test 1', function () {
        var app = new module.AppFactory.getInstance().create();
        assert.throws(function () {
            app.throwTest1();
        }, "throwTest1");
    });
    it('throw test 2', function () {
        var app = new module.AppFactory.getInstance().create();
        assert.throws(function () {
            app.throwTest1();
        }, "throwTest1");
    });
    it('throw test 3', function () {
        var app = new module.AppFactory.getInstance().create();
        assert.throws(function () {
            app.throwTest1();
        }, "throwTest1");
    });
});
