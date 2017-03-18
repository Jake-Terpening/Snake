//NAME              ID
//Jake Terpening    21945519
//Whitney Tran      77187709
//Daniel Williams   77402284
//Tristan Bock      50665011


var Server;

var SPACESIZE = 20;
var col = 12, row = 12; // store size of board

var state_str = "";
var IncMessage = "";

var p1Name = "", p2Name = "";
var clientSnake; // client's snake
var score1 = 0, score2 = 0;

var KEY_LEFT = 37, KEY_UP = 38, KEY_RIGHT = 39, KEY_DOWN = 40; //keyboard [left,up,right,down]
var A_KEY = 65, W_KEY = 87, D_KEY = 68, S_KEY = 83;  //keyboard [a,w,d,s]

var offset = 0;

//sends message from client to update server
function send(text)
{
    setTimeout(function(){
        Server.send('message', text);       
    }, Math.floor((Math.random() * 390) + 10))
}

function connect()
{
    	Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    	Server.connect();

    	Server.bind('open', function ()
	{
        	send("START:" + document.getElementById('playerid').value + ":DEFAULT");
        	document.getElementById("cntBtn").disabled = true;
    	});

    	Server.bind('close', function (data)
	{
        	document.getElementById("cntBtn").disabled = false;
    	});

    //receives message back from server to update client
    	Server.bind('message', function (message)
	{
    	    IncMessage = message.split(':'); //which ever format the message comes in

    	    if (IncMessage[0] == "START") // START:GameBoard:clientSnake:p1Name:p2Name
    	    {
    	        state_str = IncMessage[1];
    	        clientSnake = IncMessage[2];
    	        p1Name = IncMessage[3];
    	        p2Name = IncMessage[4];
    	        offset = IncMessage[5];
    	        main();
    	    }

    	    else if (IncMessage[0] == "UPDATE") // UPDATE:GameBoard:score1:score2:offset
    	    {
    	        state_str = IncMessage[1];
    	        score1 = IncMessage[2];
    	        score2 = IncMessage[3];
    	        offset = IncMessage[4];
    	    }
    	});
}

//Get which key was pressed
//var A_KEY = 65, W_KEY = 87, D_KEY = 68, S_KEY = 83;  //keyboard [a,w,d,s]
function getKeypress(key)
{
    var date = new Date();
    var milliseconds = date.valueOf();

    if (key.keyCode == A_KEY)
        send("MOVE:a:" + milliseconds);
    if (key.keyCode == W_KEY)
        send("MOVE:w:" + milliseconds);
    if (key.keyCode == D_KEY)
        send("MOVE:d:" + milliseconds);
    if (key.keyCode == S_KEY)
        send("MOVE:s:" + milliseconds);
}

//updates server on current client state
function update()
{
	getKeypress();
}

function init()
{
        draw_by_str(state_str);
}

function loop()
{
        update();
        draw_by_str(state_str);
        window.requestAnimationFrame(loop);
}

function draw_by_str(state_str)
{
    var c_x=0;        //current x-pos
    var c_y = 0;        //current y-pos
    var c_w = canvas.width / col; //pixel width
    var c_h = canvas.height / row; //pixel height

    for (var i = 0; i < state_str.length; i++)      //iterates through the state string (created in gamestate by state_str())
    {
        var c = state_str.charAt(i);
        context.beginPath();
        context.rect(c_x, c_y, c_w, c_h) // (top-right x coor of rect, top-right y, pixel width, pixel height)

        //border
        if(c == "X")
        {
            context.fillStyle = "black";        
            context.fill();
            c_x += 20;                          //go to next tile
        }

        //new row
        if(c == "-")                            
        {
            c_x = 0; 
            c_y += 20;
        }

        //empty space
        if(c == "0")
        {
            context.fillStyle = "green";
            context.fill();
            c_x += 20;                          //go to next tile
        }

        //contains player 1
        if (c == "1") {
            context.fillStyle = "red";
            context.fill();
            c_x += 20;                          //go to next tile
        }

        //contains player 2
        if (c == "2") {
            context.fillStyle = "blue";
            context.fill();
            c_x += 20;                          //go to next tile
        }

        //contains food
        if (c == "3") {
            context.fillStyle = "yellow";
            context.fill();
            c_x += 20;                          //go to next tile
        }
            
    }
    context.fillStyle = "white";
    context.fillText(p1Name + ": " + score1, 10, canvas.height-5);
    context.fillText(p2Name + ": " + score2, canvas.width - 100, canvas.height - 5);
    context.fillText("latency" + ": " + offset + " ms", 10, 15);
}


function main()
{
        canvas = document.createElement("canvas");
        canvas.width = 12 * 20; // add 2 extra for border space
        canvas.height = 12 * 20; 
        context = canvas.getContext("2d");
        document.body.appendChild(canvas);
        context.font = "16px Arial";
        
        document.addEventListener("keydown", getKeypress, false);

        init();
	    loop();
}
