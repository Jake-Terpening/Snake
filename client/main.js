//NAME              ID
//Jake Terpening    21945519
//Whitney Tran      77187709
//Daniel Williams   77402284
//Tristan Bock      50665011


var Server;

var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");
var SPACESIZE = 20;

var snake1 = "", snake2 = "";
var clientSnake; // client's snake
var score1 = 0, score2 = 0;

var KEY_LEFT = 37, KEY_UP = 38, KEY_RIGHT = 39, KEY_DOWN = 40; //keyboard [left,up,right,down]
var A_KEY = 65, W_KEY = 87, D_KEY = 68, S_KEY = 83;  //keyboard [a,w,d,s]

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
	    if(server.get() == "messageGrow"){
		    this.grow();
	    }
	    if(server.get() == "messageCollision"){
		    this.die();
	    }
	    if(server.get() == "messageOkDir"){
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
    Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    Server.connect();

    Server.bind('open', function () {
        document.getElementById("cntBtn").disabled = true;
        send("SETUP:" + document.getElementById('playerid').value);
        main();
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
	    send(getKeypress());
    }

    //yup
    function draw() {
        //visual goodness

        ctx.fillStyle = "#000";
        ctx.fillText("P1: " + score1, 5, 20);
        ctx.fillText("P2: " + score2, 5, 40);
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
        canvas = document.createElement("canvas");
        canvas.width = 480;
        canvas.height = 480;
        ctx = canvas.getContext("2d");
        document.body.appendChild(canvas);
        ctx.font = "16px Arial";

        init();
        loop();
    }
}

//Get which key was pressed
document.addEventListener("keydown", keyDownHandler, false);
function getKeypress(e) {
	//W/UP, A/LEFT, S/DOWN, D/RIGHT
    //Here goes stuff
}