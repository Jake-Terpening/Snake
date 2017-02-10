//NAME              ID
//Jake Terpening    21945519
//Whitney Tran      77187709
//Daniel Williams   77402284
//Tristan Bock      50665011


// Your code here!
var server;

var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");

var p1 = new Snake();
p1.create("player1")        //sets up player1
var p2 = new Snake();
p2.create("player2")        //sets up player2
var f = new Food();
f.setLoc();                 //spawns food in a random location

var SPACESIZE = 20;


//snake object
function Snake() {
    this.x = 0;
    this.y = 0;
    this.xspd = 0;
    this.yspd = 0;
    this.id = "";
    this.score = 0;
    this.ctrl_arr = [0, 1, 2, 3];
    this.tail = []

    this.getx = function () {
        return this.x;
    }

    this.gety = function () {
        return this.y;
    }

    this.getcontrols = function () {
        return this.ctrl_arr;
    }

    this.direction = function (x, y)  //moves in direction {left = (-1,0), up = (0,-1), right = (1,0), down = (0,1)}
    {
        this.xspd = x;
        this.yspd = y;
    }

    this.create = function (id)      //creates a snake, given a player id
    {
        if (id == "player1")        //player1 is in the top left corner and moves down
        {
            this.x = 2 * SPACESIZE;
            this.y = 2 * SPACESIZE;
            this.direction(0, 1)
            this.id = "player1";
            this.ctrl_arr = [65, 87, 68, 83];  //keyboard [a,w,d,s]
        }

        if (id == "player2")        //player2 is in the bottom right corner and moves up
        {
            this.x = 22 * SPACESIZE;
            this.y = 22 * SPACESIZE;
            this.direction(0, -1)
            this.id = "player2";
            this.ctrl_arr = [37, 38, 39, 40];  //arrow keys [left,up,right,down]
        }
    }

    this.eat = function (myfood)           //eats a food, changing the foods location and incrementing the score
    {
        this.score == this.score + 1;
        myfood.setLoc();
    }

    this.check_death = function () {
        if (this.x < 0 || this.y < 0 || this.x >= 480 || this.y >= 480)
            return true;
        return false;
    }

    this.update = function () {
        this.x = this.x + this.xspd * SPACESIZE;
        this.y = this.y + this.yspd * SPACESIZE;

        for(var i = 0; i<this.score-1; i++)
        {
            this.tail[i] = this.tail[i + 1];            //shift tail down
        }
        this.tail[this.score - 1] = [this.x, this.y];
    }

    this.show = function () {
        ctx.beginPath();
        ctx.rect(this.x, this.y, SPACESIZE, SPACESIZE);
        if (this.id == "player1")
            ctx.fillStyle = "#FF0000";
        else
            ctx.fillStyle = "#00FF00"
        for (var i = 0; i < this.score; i++)
        {
            ctx.rect(this.tail[i][0], this.tail[i][1], SPACESIZE, SPACESIZE);       //draw tails
        }
        ctx.fill();
        ctx.closePath();
    }
}

//food object
function Food() {
    this.x = 0;
    this.y = 0;

    this.getx = function () {
        return this.x;
    }

    this.gety = function () {
        return this.y;
    }

    this.setLoc = function ()        //puts the food in a random space
    {
        this.x = Math.round(Math.random(24)) * SPACESIZE;
        this.y = Math.round(Math.random(24)) * SPACESIZE;
    }

    this.show = function () {
        ctx.beginPath();
        ctx.rect(this.x, this.y, SPACESIZE, SPACESIZE);
        ctx.fillStyle = "#0000FF";
        ctx.fill();
        ctx.closePath();
    }
}

//controls snakes according to key presses
document.addEventListener("keydown", keyDownHandler, false);
function keyDownHandler(e) {
    if (e.keyCode == p1.getcontrols()[0])
        p1.direction(-1, 0)
    else if (e.keyCode == p1.getcontrols()[1])
        p1.direction(0, -1)
    else if (e.keyCode == p1.getcontrols()[2])
        p1.direction(1, 0)
    else if (e.keyCode == p1.getcontrols()[3])
        p1.direction(0, 1)
    else if (e.keyCode == p2.getcontrols()[0])
        p2.direction(-1, 0)
    else if (e.keyCode == p2.getcontrols()[1])
        p2.direction(0, -1)
    else if (e.keyCode == p2.getcontrols()[2])
        p2.direction(1, 0)
    else if (e.keyCode == p2.getcontrols()[3])
        p2.direction(0, 1)
}

//draws the game area
function draw() {
    var gameOver = false;

    while (!gameOver) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);       //clear canvas at the start of each loop
        ctx.beginPath();
        p1.show();
        p2.show();
        f.show();


        p1.update();
        p2.update();

        if (p1.getx() == f.getx() && p1.gety() == f.gety())             //checks for p1 collision with food
            p1.eat(f);

        if (p2.getx() == f.getx() && p1.gety() == f.gety())             //checks for p2 collision with food
            p1.eat(f);

        if ((p1.getx() == p2.getx() && p1.gety() == p2.gety()) || p1.check_death() || p2.check_death()) //checks for player collision
            gameOver = true;                                                                //with wall or other player
        ctx.endpath()
    }
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
}
