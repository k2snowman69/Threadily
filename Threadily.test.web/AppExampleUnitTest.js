var assert = require('assert');
var module = require('./Threadily.SampleObjects');

describe('App Example', function () {

    it('Factory creates App, App creates Business, Business reads products', function (done) {
        var app = new module.AppFactory.getInstance().create();

        var newBusiness = null;
        var readBusinesses = function () {
            var readProductHandle = newBusiness.isProductsPending.subscribe(new module.ISubscribeHandleBoolCallback.implement({
                onChange(isPending) {
                    if (!isPending) {
                        readProductHandle = null;
                        console.log("Reading products complete");
                        done();
                    }
                }
            }));
            console.log("Reading products");
            newBusiness.readProductsAsync(0, 20, "");
        }

        var businesscallback = new module.ISubscribeHandleBoolCallback.implement({
            onChange(isPending) {
                if (!isPending) {
                    createBusinessHandle = null;
                    console.log("Created business complete");
                    newBusiness = app.createdBusiness.get();
                    readBusinesses();
                }
            }
        });
        var createBusinessHandle = app.isCreateBusinessPending.subscribe(businesscallback);
        console.log("Creating business");
        app.createBusinessAsync("Business");
    });
});
