//NAME              ID
//Jake Terpening    21945519
//Whitney Tran      77187709
//Daniel Williams   77402284
//Tristan Bock      50665011


var Server = fancyWebSocket;

var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");
var SPACESIZE = 20;


function Snake(posx, posy, dir) {
    this.direction = dir,
    this.body = [{x:posx, y:posy}],
    this.head = this.body[0],

    //it eats, it grows
    this.grow = function (posx, posy) {
        this.body.unshift({ x: posx, y: posy });
        this.head = this.body[0];
    }

    //need to complete
    this.update = function () {
	if(Server.get() == "messageGrow"){
		this.grow();
	}
	if(Server.get() == "messageCollision"){
		this.die();
	}
	if(Server.get() == "messageOkDir"){
		this.direction = keypress;
	}
    }
}

//distinguishes which client ID is which player
function whichSnake(snakeID) {
    if (snakeID == p1)
        return p1;
    else if (snakeID == p2)
        return p2;
}

//sends message from client to update server
function send(text) {
    Server.send('message', text);           
}

function connect() {
    send('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    Server.connect();

    Server.bind('open', function () {
        document.getElementById("cntBtn").disabled = true;
    });

    Server.bind('close', function (data) {
        document.getElementById("cntBtn").disabled = false;
    });

    //receives message back from server to update client
    Server.bind('message', function (message) {
        var messageArr = message.split(); //which ever format the message comes in
    });

    //updates server on current client state
    function update() {
	Server.send(getKeypress());
    }

    //yup
    function draw() {
        //visual goodness
    }

    function loop() {
        update();
        draw();

        window.requestAnimationFrame(loop);
    }

    function init() {
        //intialize board
    }

    function main() {
        init();
        loop();
    }
}

//Get which key was pressed
function getKeypress() {
	//W/UP, A/LEFT, S/DOWN, D/RIGHT
	//Here goes stuff
}