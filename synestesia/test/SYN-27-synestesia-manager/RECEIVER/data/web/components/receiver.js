Vue.component('receiver-component', { 
    data: function() {
        return {
            connectError: true,
            connecting: false,
            attempted: false,
            SSID: "",
            PSWD: ""
        }
    },
    methods: {
        connect () {
            this.attempted = true;
            this.connecting = true;

            var params = "target-ssid=${ssid}&target-password=${pswd}";
            
            var body = renderTemplate(params, {ssid: this.SSID, pswd: this.PSWD});
            
            requestToServer('POST', "/connect", this.handleResponse, body, [{header: "Content-Type", value: "application/x-www-form-urlencoded"}]);
        },

        handleResponse (responseText) {
            this.connecting = false;
            this.connectError = responseText !== "OK";
        }
    },
    template: `
        <div id="receiver_configuration" class="mx-5">
            <div class="d-flex justify-content-between">
                <h3 class="display-4">Connect to</h3> 
                <span v-if="attempted" class="badge align-self-center" :class="(connectError) ? 'badge-danger' : 'badge-success'">
                    {{(connectError) ? "Not connected" : "Connected"}}
                </span>
            </div>
            <form class="px-5 col-md-5 m-auto" action="/connect" method="post">
                <div class="form-group">
                    <label>Target SSID</label>
                    <input type="text" class="form-control" v-model="SSID" id="target-ssid" placeholder="SSID">
                </div>
                <div class="form-group">
                    <label class="white">Target Password</label>
                    <input type="text" class="form-control" v-model="PSWD" id="target-password" placeholder="Password">
                </div>
            </form>
            <button class="btn btn-primary" @click="connect()" :disabled="connecting">
                <span v-if="connecting" class="spinner-border spinner-border-sm"></span>
                Connect
            </button>
        </div>
    `
});