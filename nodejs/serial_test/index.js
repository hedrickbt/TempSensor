var SerialClass = require('serialport')
var Serial = SerialClass.SerialPort
var Port = process.argv[2]

if (Port === 'undefined' || Port == null){
console.log('Error: Need port')
return -1;
}

var Com = new Serial(Port, {
baudrate : 115200,
parser:SerialClass.parsers.readline('\r\n')
})


Com.on('open', function(){
// this is when the serial port is open
console.log('Connetion is open')
})

Com.on('data', function(source){
// this is when serial data enters the system
console.log(source)
})

process.stdin.setEncoding('utf8')

process.stdin.on('readable',function(){
var chunk = process.stdin.read()
if(chunk !== null){
Com.write(chunk.replace('\n','\r'))
}
})
