var WebSocketServer = require('websocket').server;
var http = require('http');
var http2 = require('http');

var setter_server = http.createServer(function(request,response) {

});
var getter_server = http2.createServer(function(request,response) {

});

setter_server.listen(5250,function(){});
getter_server.listen(5251,function(){});

wsSetServer = new WebSocketServer({
    httpServer: setter_server,
    //protocol: 'of-protocol'
});

wsGetServer = new WebSocketServer({
    httpServer: getter_server,
    //protocol: 'of-protocol'
});

var MSG_TYPE_STREAM = 0;
var MSG_TYPE_TILES  = 1;

var LOC_ID_TC = 100;
var LOC_ID_LIB = 101;

var libraryJson={
  'libraryJson_undefined': -1
};
var libraryTiles={
  'libraryTiles_undefined': -1
};

var tcJson={
  'tcJson_undefined': -1
};
var tcTiles={
  'tcTiles_undefined': -1
};


var setterConnection;
var getterConnection;
console.log('Listening on port 5250 for all SET commands');
console.log('Listening on port 5251 for all GET commands');

wsSetServer.on('request', function(request)
{
    console.log('Got connection request from '+request);
    setterConnection = request.accept('of-protocol', request.origin);
    console.log('Accepted connection.');
    setterConnection.on('message', function(message)
    {
      if(message.type === 'utf8')
      {
        //console.log(message.utf8Data);
      try 
      {
        var command = JSON.parse(message.utf8Data);
        if(     command.ORIGIN == LOC_ID_LIB  && command.TYPE == MSG_TYPE_STREAM)
        {
            // console.log('library stream');
            libraryJson = command;
        } 
        else if(command.ORIGIN == LOC_ID_LIB  && command.TYPE == MSG_TYPE_TILES )
        { 
            // console.log('library tile');
            libraryTiles = command;     
        } 
        else if(command.ORIGIN == LOC_ID_TC   && command.TYPE == MSG_TYPE_STREAM)
        {
            // console.log('theatre stream');
            tcJson = command;
        } 
        else if(command.ORIGIN == LOC_ID_TC   && command.TYPE == MSG_TYPE_TILES )
        {
            // console.log('theatre tile');
            tcTiles = command;
        } 
        else console.log("Received unknown message: "+message.utf8Data);
      }
      catch(e)
      {
        console.log("Error: "+e);
      } 
    }}
)});

wsGetServer.on('request',function(request)
{
  getterConnection = request.accept('of-protocol', request.origin);
  console.log("Saw connection on GET server");
  getterConnection.on('message', function(message)
  {
    console.log("Saw message: "+message.utf8Data);
    if(message.type === 'utf8')
    {
    console.log(message.utf8Data);
    try{
      var command = JSON.parse(message.utf8Data);
      switch(command.ORIGIN)
      {

        case LOC_ID_LIB:
        getterConnection.send(JSON.stringify(libraryJson));
        getterConnection.send(JSON.stringify(tcTiles));
        break;

        case LOC_ID_TC:
        getterConnection.send(JSON.stringify(tcJson));
        getterConnection.send(JSON.stringify(libraryTiles));
        break;

        default:
        console.log("Received packet I don't understand from "+getterConnection.origin+"Containing "+message);
      }
    }
    catch(e){console.log("An error occured: "+e);}
    }
  }
)});
