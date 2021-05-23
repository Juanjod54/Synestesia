
Vue.component('global-component', {
    data: function() {
        return {
            validations: {
                component: true,
                ssid: true,
                pswd: true
            },
            delimiter: '$',
            SSID: "",
            PSWD: ""
            //ADM: ""
        }
    },
    created: function () {    
        requestToServer('GET', "/global-data", this.onFetchGlobalData, null);
    },
    computed : {
        validSSID () {
            this.validations.ssid = this.SSID !== "" && this.SSID.length < 12;
            this.validations.component = this.validations.ssid && this.validations.pswd;
            this.$emit("validation", this.validations.component);
            return this.validations.ssid;
        },
        validPSWD() {
            this.validations.pswd = this.PSWD == "" || this.PSWD.length == 8;
            this.validation = this.validations.ssid && this.validations.pswd;
            this.$emit("validation", this.validations.component);
            return this.validations.pswd;
        }
    },
    methods: {
        
        marshallValues: function () {
            var body = "";
            body += "SSID:" + this.SSID + this.delimiter;
            body += "PSWD:" + this.PSWD; // + this.delimiter;
            //body += "ADM:" + this.ADM;

            return body;
        },

        onFetchGlobalData: function(text) {
            if (text !== null) {
                var keyValuesList = text.split(this.delimiter);
                for (var i = 0; i < keyValuesList.length; i++) {
                    var keyValue = keyValuesList[i].split(":"); 
                    if (keyValue[0] === "SSID") this.SSID = keyValue[1];
                    else if (keyValue[0] === "PSWD") this.PSWD = keyValue[1];
                    //else if (keyValue[0] === "ADM") this.ADM = keyValue[1];
                }
            }
        }
    },
    template: `
        <div id="global_configuration" class="mx-5">
            <h3 class="display-4 align-self-center">Global</h3> 
            <form class="px-5 col-md-5 m-auto">
                <div>
                    <label>SSID</label>
                    <input type="text" class="form-control" 
                        :class="(validSSID) ? 'is-valid' : 'is-invalid'" 
                        v-model="SSID" placeholder="SSID">
                    <p v-if="!validSSID"><small>SSID must be 12 characters or less</small></p>
                </div>
                <div>
                    <label class="white">Password</label>
                    <input type="text" class="form-control" 
                        :class="(validPSWD) ? 'is-valid' : 'is-invalid'"
                        v-model="PSWD" id="password" placeholder="Password">
                    <p v-if="validPSWD"><small>Password can be empty. WiFi will be open</small></p>
                    <p v-if="!validPSWD"><small>Password must be 8 digits long</small></p>
                </div>
                <!-- <div>
                    <label class="white">Admin Password</label>
                    <input type="password" class="form-control" v-model="ADM" id="adm_pass" placeholder="Password">
                </div> -->
            </form>
        </div>
    `
});
