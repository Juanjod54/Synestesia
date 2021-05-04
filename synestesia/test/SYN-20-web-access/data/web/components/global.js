
Vue.component('global-component', {
    data: function() {
        return {
            SSID: "",
            PSWD: "",
            ADM: ""
        }
    },
    created: function () {    
        requestToServer('GET', "/global-data", this.onFetchGlobalData, null);
    },
    methods: {
        save: function() {
            requestToServer('POST', "/global-data", null, this.marshallValues());
        },

        marshallValues: function () {
            var body = "";
            body += "SSID:" + this.SSID + ";";
            body += "PSWD:" + this.PSWD + ";";
            body += "ADM:" + this.ADM;

            return body;
        },

        onFetchGlobalData: function(text) {
            if (text !== null) {
                var keyValuesList = text.split(";");
                for (var i = 0; i < keyValuesList.length; i++) {
                    var keyValue = keyValuesList[i].split(":"); 
                    if (keyValue[0] === "SSID") this.SSID = keyValue[1];
                    else if (keyValue[0] === "PSWD") this.PSWD = keyValue[1];
                    else if (keyValue[0] === "ADM") this.ADM = keyValue[1];
                }
            }
        }
    },
    template: `
        <div id="global_configuration">
            <form>
                <div>
                    <label>SSID</label>
                    <input type="text" class="form-control" v-model="SSID" id="ssid" placeholder="SSID">
                </div>
                <div>
                    <label class="white">Password</label>
                    <input type="text" class="form-control" v-model="PSWD" id="password" placeholder="Password">
                </div>
                <div>
                    <label class="white">Admin Password</label>
                    <input type="password" class="form-control" v-model="ADM" id="adm_pass" placeholder="Password">
                </div>
            </form>
        </div>
    `
});
