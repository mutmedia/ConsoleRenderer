var _ = require("lodash");
var bmp = require("bmp-js");
var fs = require('fs');
var Jimp = require("jimp");

var inLocation = "./TestImages/";
var outLocation = "./GeneratedCode/";
var filename = process.argv[2];

Jimp.read(inLocation + filename + ".bmp", function (err, consoleColors) {
  colorsList = [];

  consoleColors.scan(0, 0, consoleColors.bitmap.width, consoleColors.bitmap.height, function (x, y, idx) {
      var blue   = this.bitmap.data[ idx + 0 ];
      var green = this.bitmap.data[ idx + 1 ];
      var red = this.bitmap.data[ idx + 2 ];
      var c = {
        color:(red << 16) + (green << 8) + blue,
        x: x, y: y
      };

      colorsList.push(c);
    });


    colorListtoCppCode = _.reduce(colorsList, function(result, c) {
        return result += "img->SetColor(" + c.x + ", "+ c.y+ ", { "+ c.color+ " });\n";
    }, "void Create" + filename + "(Image * img) {\n");
    colorListtoCppCode += "}";
    fs.writeFile(outLocation + filename + ".txt", colorListtoCppCode);

  });
