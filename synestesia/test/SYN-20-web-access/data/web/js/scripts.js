function requestToServer(method, url, callback, body, headers) { var xmlHttp = new XMLHttpRequest(); xmlHttp.onload = function() { if (callback !== null && callback !== undefined) { if (xmlHttp.readyState == 4 && xmlHttp.status == 200) { callback(xmlHttp.responseText); } else { callback(null); }}}; xmlHttp.open(method, url, true); 

if (headers !== undefined && headers !== null) {
    for (var i = 0; i < headers.length; i++) { xmlHttp.setRequestHeader(headers[i].header, headers[i].value); }
}

xmlHttp.send(body);}
function renderTemplate(template, object) { return template.replace(/\${(\w+)}/g, (_, v) => object[v]); }
