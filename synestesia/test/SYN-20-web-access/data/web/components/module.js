
Vue.component('ModuleComponent', {
    data: function () { 
        return {
            lights: [],
            notes_map: ["do", "re", "mi", "fa", "sol", "la", "si", "DO", "RE", "MI", "FA", "SOL", "LA", "SI"],
            connection_template: "LIGHT:;CONN:${red},${green},${blue};",
            color_template: "${note}:${red},${green},${blue};"
        }
    },
    created: function () {    
        requestToServer('GET', "/module-data", this.onFetchModuleData, null);
    },
    methods: {
        save: function() {
            requestToServer('POST', "/module-data", null, this.marshallValues());
        },

        marshallValues: function () {
            var body = "";
            for (var li = 0; li < this.lights.length; li++) {
                var light = this.lights[li];
                body += renderTemplate(this.connection_template, light.CONN);
                for (var ci in light.colors) {
                    body += renderTemplate(this.color_template, light.colors[ci]);
                }
            }

            return body.slice(0, -1);
        },

        onFetchModuleData: function(text) {
            var light;
            var light_flag = 0;

            if (text === null) return; 
    
            var keyValuesList = text.split(";");

            for (var i = 0; i < keyValuesList.length; i++) {
                var key_and_value = keyValuesList[i].split(":");
                
                var key = key_and_value[0];
                var value = key_and_value[1];

                if (key === "LIGHT" && light_flag === 0) {
                    light = {};
                    light_flag = 1;
                    this.lights.push(light);
                }
                else if (key === "CONN" && light_flag === 1) {
                    var connections = value.split(",");
                    light.CONN = {
                        red: connections[0],
                        green: connections[1],
                        blue: connections[2]
                    }
                    
                    light_flag = 0;
                }
                else if (parseInt(key) > 0 && light_flag === 0) {
                    var colors = value.split(",");
                    if (light.colors === undefined) light.colors = {};
                    
                    light.colors[key] = {
                        note: key,
                        red: colors[0],
                        green: colors[1],
                        blue: colors[2]
                    }
                }
            }
        }
    },
    template: `
        <div id="rgb_light_configuration">
            <div v-for="light in lights" class="jumbotron">
                <h2 class="display-4">Light</h2>
                <div>
                    <h3 class="display-6">Connections</h2>
                    <div class="d-flex flex-column">
                        <div class="d-flex justify-content-md-center" v-for="(value, connection) in light.CONN">
                            <label class="col-md-1"> {{ connection }} </label>
                            <input class="col-sm-1" v-model="value">
                        </div>
                    </div>
                    <div>
                        <h3 class="display-6">Colors</h2>
                        <div class="d-flex flex-column">
                            <div v-for="(color, note) in light.colors">
                                <h4>{{ notes_map[note -1] }}</h4>
                                <div class="d-flex justify-content-md-center" v-for="(color_value, color_name) in color">
                                    <label class="col-md-1">{{ color_name }}</label>
                                    <input class="col-sm-1" v-model="color_value">
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    `
});
