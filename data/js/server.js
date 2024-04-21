
// Start "INDEX"
function StartIndex() {
    getConfig("index");   	   // Load configuration or  config.json 	
    getUpdate();
}

// Start "Network_Config"
function StartNetConfig() {
    getConfig("network");
}

// Start "System"
function StartSystem() {
    getConfig("system");
}

// Start "System"
function StartInfo() {
    getConfig("shortcut");
}


function getConfig(information) {
    // 1. Создаём новый XMLHttpRequest-объект с именем o
    let o = new XMLHttpRequest(); // Функция конструктор. При помощи встроенного в браузер обьекта XMLHttpRequest делаем HTTP запрос к серверу без перезагрузки страницы. 
    // 2. Настраиваем параметры запроса ( HTTP метод "GET" /куда запрос: URL или файл/)
    // o.open("GET", "config.json", 1); // поставить "/config.json"
    o.open("GET", "config.json", 1); // поставить "/config.json"
    // 3. Отправка инициализированного запроса 
    o.send();
    // 4. Этот код сработает после того, как мы получим ответ сервера
    o.onload = function () {
        // 5. Декодируем полученный текст ответа от сервера и запишем его в переменную ConfigFilePars
        jsonBuf = JSON.parse(o.responseText);

        // 6. Цикл для перебора всех свойств в строке ConfigFilePars
        for (let key in jsonBuf) {
            // Выполняется код внутри try, и если в нем нет ошибок - catch игнорируется. 
            try {
                let { type } = input = document.getElementById(key);
                if (type == "checkbox") input.checked = jsonBuf[key] > 0;
                else input.value = jsonBuf[key];
            }
            catch (e) { };

            if (information == "index") {
                document.getElementById("sn").value = jsonBuf["sn"];
                document.getElementById("firmware").value = jsonBuf["firmware"];
            }

     
        }

        // if (jsonBuf["sn"] == 0) {
        //     info_msg = "Внимание! Первый запуск устройства. Необходимо установить серийный номер и параметры конфигурации.";
        //     alert(info_msg);
        // }
    }
}

function getUpdate() {
    let xml = new XMLHttpRequest();
    xml.open("GET", "update.json", 1);
    xml.send();
    xml.onload = function () {
        let Pars = JSON.parse(xml.responseText);
        document.getElementById("date").value = Pars["d"];
        document.getElementById("time").value = Pars["t"];
    }
    // setTimeout("getUpdate()", 1000);
}

function BTimeSet() {
    let xml = new XMLHttpRequest();
    let buf = "?";
    buf += "T=" + document.getElementById("time").value + "&";
    buf += "D=" + document.getElementById("date").value;

    xml.open("GET", "SysUPD" + buf, 1);
    console.log(xml);
    xml.send();
    alert("Настройки сохранены.");
}

function BTimeAutoSet() {
    let xml = new XMLHttpRequest();
    let now = new Date();

    let buf = "?";
    buf += "T=" + now.getHours() + ":" + now.getMinutes() + "&";
    buf += "D=" + now.getFullYear() + "-" + (now.getUTCMonth() + 1) + "-" + now.getDate();

    xml.open("GET","SysUPD" + buf,true);
    xml.send();
    setTimeout("reload()",1000);
    alert("Настройки сохранены.");

}

function BWiFiUPD() {

    let xml = new XMLHttpRequest();

    result = confirm("Вы уверены, что хотите изменить имя точки доступа / пароль табло?");

    if (result) {

        let buf = "?";
        buf += "ssid=" + document.getElementById("ssid_name").value + "&";
        buf += "pass=" + document.getElementById("pass").value;

        xml.open("GET", "WiFiUPD" + buf, 1);
        xml.send();
        mcu_restart();
        // console.log(xml);
        let msg = "Изменения сохранены! Табло уже перезагружается (займёт 15сек).\r\n";
        msg += "Запишите новые настройки:\r\n";
        msg += "Имя сети: ";
        msg += document.getElementById("ssid_name").value + "\r\n";
        msg += "Пароль: ";
        msg += document.getElementById("pass").value + "\r\n";
        msg += "\r\n";
        msg += "Если пароль будет утерян, можно сбросить все настройки до";
        msg += " заводских (удерживать кнопки №1 + №5 (две крайние одновременно) в течение 20сек)";

        alert(msg);
    }
}

function reload() {
    window.location.reload();
}

function mcu_restart() {
    let xml = new XMLHttpRequest();
    xml.open("GET", "BRBT", 1);
    console.log(xml);
    xml.send();

}

function FactoryReset() {
    result = confirm("Вы уверены, что хотите сбросить все установленные настройки?");
    if (result) {

        let xml = new XMLHttpRequest();
        xml.open("GET", "BFRST", 1);
        console.log(xml);
        xml.send();
        mcu_restart();

        let msg = "Изменения сохранены! Табло уже перезагружается (займёт 15сек).\r\n";
        msg += "Настройки установлены по умолчанию:\r\n";
        msg += "Имя сети: ";
        msg += "retra0846 \r\n";
        msg += "Пароль: ";
        msg += "retra0846zxc\r\n";
        alert(msg);

    }
}