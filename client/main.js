//NAME              ID
//Jake Terpening    21945519
//Whitney Tran      77187709
//Daniel Williams   77402284
//Tristan Bock      50665011


var Server;

var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");
var SPACESIZE = 20;
var col = 0, row = 0; // store size of board
var grid;
var IncMessage = "";

var p1Name = "", p2Name = "";
var clientSnake; // client's snake
var score1 = 0, score2 = 0;

var KEY_LEFT = 37, KEY_UP = 38, KEY_RIGHT = 39, KEY_DOWN = 40; //keyboard [left,up,right,down]
var A_KEY = 65, W_KEY = 87, D_KEY = 68, S_KEY = 83;  //keyboard [a,w,d,s]

function Snake(posx, posy, dir) {
    this.direction = dir,
    this.body = [{x:posx, y:posy}],
    this.head = this.body[0],

    // it eats, it grows
    this.grow = function (posx, posy) {
        this.body.unshift({ x: posx, y: posy });
        this.head = this.body[0];
    }

    // used when shifting snake along board, pops end bit off
    this.shed = function () {
        return this._queue.pop();
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
        send("START:" + document.getElementById('playerid').value);
        document.getElementById("cntBtn").disabled = true;
    });

    Server.bind('close', function (data) {
        document.getElementById("cntBtn").disabled = false;
    });

    //receives message back from server to update client
    Server.bind('message', function (message) {
        IncMessage = message.split(':'); //which ever format the message comes in
    });
}

//Get which key was pressed
//var A_KEY = 65, W_KEY = 87, D_KEY = 68, S_KEY = 83;  //keyboard [a,w,d,s]
function getKeypress() {
	//W/UP, A/LEFT, S/DOWN, D/RIGHT
    //Here goes stuff
    if (e.keyCode == A_KEY || e.keyCode == KEY_LEFT)
        send("MOVE:a");
    else if (e.keyCode == W_KEY || e.keyCode == KEY_UP)
        send("MOVE:w");
    else if (e.keyCode == D_KEY || e.keyCode == KEY_RIGHT)
        send("MOVE:d");
    else if (e.keyCode == S_KEY || e.keyCode == KEY_DOWN)
        send("MOVE:s");
}

//updates server on current client state
function update() {
	send(getKeypress());
	//get message from server
	Snake.update();
}

function handleMessage() {
        if (IncMessage[0] == "START") // START:col:row:clientSnake
        {
            col = IncMessage[1];
            row = IncMessage[2];
            clientSnake = IncMessage[3];
            p1Name = IncMessage[4];
            p2name = IncMessage[5];
            main();
        }

        else if (IncMessage[0] == "UPDATE") // STATE:foodLoc:player1Loc:player2Loc:score1:score2:FoodEaten(0/1/2)
        {
		//grid(thing) = foodLoc;
		//etc.
        }
}

//yup
function draw() {
        //visual goodness
	ctx.fillStyle = "#000";
        ctx.fillText(p1Name + score1, 5, 20);
	ctx.fillText(p2name + score2, 5, 40);
}

function loop() {
        update();
        draw();
        window.requestAnimationFrame(loop);
}

function init() {
        // empty board marked with 0
        for (var x = 0; x < col; x++)
        {
            for (var y = 0; y < row; y++)
            {
                grid[x][y] = 0;
            }
        }
}

function main() {
        canvas = document.createElement("canvas");
        canvas.width = col * 20;
        canvas.height = row * 20;
        ctx = canvas.getContext("2d");
        document.body.appendChild(canvas);
        ctx.font = "16px Arial";

        document.addEventListener("keydown", keyDownHandler, false);

        init();
	loop();
}
