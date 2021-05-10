Vue.component('ModuleComponent', {
    data: function () { 
        return {
            notes_map: ["do", "re", "mi", "fa", "sol", "la", "si", "DO", "RE", "MI", "FA", "SOL", "LA", "SI"],
            connection_template: "LIGHT:${delimiter}CONN:${red},${green},${blue}${delimiter}",
            color_template: "${note}:${red},${green},${blue}${delimiter}",
            delimiter: '$',
            lights: []
        }
    },
    created: function () {    
        requestToServer('GET', "/module-data", this.onFetchModuleData, null);
    },

    methods: {

        getColorDrop (colors) {
            return {
                "width": '25px',
                "height": '25px',
                "background-color" : `rgb(${colors.red}, ${colors.green}, ${colors.blue})`
            };
        },
        
        canAddColor(light) {
            return Object.keys(light.colors).length < this.notes_map.length;
        },

        createNewColor(light, key) {
            if (! key) {
                key = 1;
                for (var last_key in light.colors) { if (key <= last_key) key ++; }
            }
            
            this.$set(light.colors, key, {red: 0, green: 0, blue: 0 });
            
            return light.colors[key];
        },

        createNewLight() {
            var light = {
                id: this.lights.length,
                connections: {
                    red: 0,
                    green: 0,
                    blue: 0,
                },
                colors: {}
            }                    
            
            this.lights.push(light);

            return light;
        },

        checkConfiguration: function () {
            return true;
        },

        marshallValues: function () {
            var body = "";
            for (var li = 0; li < this.lights.length; li++) {
                var light = this.lights[li];
                var light_parse = {delimiter: this.delimiter, note: ci};
                Object.assign(light_parse, light.connections);
                body += renderTemplate(this.connection_template, light_parse);
                for (var ci in light.colors) {
                    var color_parse = {delimiter: this.delimiter, note: ci};
                    Object.assign(color_parse, light.colors[ci]);
                    body += renderTemplate(this.color_template, color_parse);
                }
            }

            return body.slice(0, -1);
        },

        onFetchModuleData: function(text) {
            var light;
            var lights = [];
            var light_flag = 0;

            if (text === null) return; 
    
            var keyValuesList = text.split(this.delimiter);

            for (var i = 0; i < keyValuesList.length; i++) {
                var key_and_value = keyValuesList[i].split(":");
                
                var key = key_and_value[0];
                var value = key_and_value[1];

                if (key === "LIGHT" && light_flag === 0) {
                    light_flag = 1;
                    light = this.createNewLight();
                    lights.push(light);
                }
                else if (key === "CONN" && light_flag === 1) {
                    var connections = value.split(",");

                    this.$set(light.connections, "red", connections[0]);
                    this.$set(light.connections, "green", connections[1]);
                    this.$set(light.connections, "blue", connections[2]);                    
                    light_flag = 0;
                }
                else if (parseInt(key) > 0 && light_flag === 0) {
                    var colors = value.split(",");
                    var light_color = this.createNewColor(light, key);

                    this.$set(light_color, "red",  colors[0]);
                    this.$set(light_color, "green",  colors[1]);
                    this.$set(light_color, "blue",  colors[2]);
                }
            }
        }
    },
    template: `
        <div id="rgb_light_configuration" class="mx-5">
            
            <div class="d-flex justify-content-between">
                <h3 class="display-4 align-self-center">Lights</h3> 
                <button class="btn btn-outline-primary align-self-center" 
                    @click="createNewLight()">

                        Add a new light
                        <span class="badge rounded-pill bg-light text-dark">{{ lights.length }}</span>

                </button>
            </div>

            <div v-for="light in lights" :key="light.id" class="px-5 border-top mx-5 my-5">
                <h2 class="display-7 mt-3">Connections</h2>
                <div class="d-flex flex-column">
                    <div class="d-flex justify-content-md-center" v-for="(value, connection) in light.connections">
                        <label class="col-md-1"> {{ connection }} </label>
                        <input class="col-sm-1" v-model="light.connections[connection]">
                    </div>
                </div>
                <div>
                    <div class="d-flex justify-content-between pb-2">
                        <h2 class="display-6 align-self-center">Colors</h2> 
                        <button class="btn btn-outline-primary align-self-center" 
                            :disabled="!canAddColor(light)"
                            @click="createNewColor(light)">

                                Add a new color
                                <span class="badge rounded-pill bg-light text-dark">{{ Object.keys(light.colors).length }}</span>

                        </button>
                    </div>
                    <div v-for="(color, note) in light.colors" 
                        class="container-sm border-top py-2">
                        
                        <div class="d-flex flex-row justify-content-between">
                            
                            <div class="align-self-center col-md-1">
                                <h4>{{ notes_map[note -1] }}</h4>
                            </div>
                            <div class="d-flex flex-sm-column"> 
                                <div class="d-flex justify-content-between" v-for="(color_value, color_name) in light.colors[note]">
                                    <label class="col-md-1">{{ color_name }}</label>
                                    <input type="range" min="0" max="255" v-model="light.colors[note][color_name]"/>
                                    <input class="col-md-2 border-0 text-center" type="text" v-model="light.colors[note][color_name]" />
                                </div>
                            </div>
                            <div class="align-self-center">
                                <div class="rounded" :style="getColorDrop(light.colors[note])"></div>
                            </div>

                        </div>

                    </div>
                </div>
            </div>
        </div>
    `
});