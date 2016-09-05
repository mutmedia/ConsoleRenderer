var _ = require("lodash");
var bmp = require("bmp-js");
var fs = require('fs');
var Jimp = require("jimp");

var imagesLocation = "DiscoveryImages";

Jimp.read(imagesLocation + '/consolecolors.bmp', function (err, consoleColors) {
    if (err) throw err;
    var colorSet = {};
    var simpleColors = [];
    consoleColors.scan(0, 0, consoleColors.bitmap.width, consoleColors.bitmap.height, function (x, y, idx) {
      //console.log(this.getPixelColor(x, y).toString(16));
      if(x % 8 === 0 && y % 8 === 0) {
        //var red   = this.bitmap.data[ idx + 0 ];
        //var green = this.bitmap.data[ idx + 1 ];
        //var blue  = this.bitmap.data[ idx + 2 ];
        var meanValue =
        {
          r: 0,
          g: 0,
          b: 0
        };
        for (var i = 0; i < 8; i++) {
          for (var j = 0; j < 8; j++) {
            var col = Jimp.intToRGBA(this.getPixelColor(x + i, y + j));
            // why is rgb switched ??
            meanValue.r += col.b;
            meanValue.g += col.g;
            meanValue.b += col.r;
          }
        }


        meanValue.r = Math.floor(meanValue.r/64);
        meanValue.g = Math.floor(meanValue.g/64);
        meanValue.b = Math.floor(meanValue.b/64);

        meanValueInt = Jimp.rgbaToInt(meanValue.r, meanValue.g, meanValue.b, 0xff);
        meanValueRGBOnly = meanValueInt>>8;
        if(!_.has(colorSet, meanValueRGBOnly))
        {
          colorSet[meanValueRGBOnly] = Math.floor(x/8) + 32*Math.floor(y/8);

        }
        if(meanValue.r === 0x00 && meanValue.g === 0xff && meanValue.b === 0xff)
        {
          console.log(meanValueInt>>8, colorSet[meanValueRGBOnly]);
        }

        simpleColors.push(meanValueInt);
      }
    });

    var output = new Jimp(32, 32, function (err, cleanImage) {
      for (var i = 0; i < 32; i++) {
        for (var j = 0; j < 32; j++) {
          cleanImage.setPixelColor(simpleColors[i + 32*j], i, j);
        }
      }
      cleanImage.write(imagesLocation + "cleanImage.bmp");
    });

    var sortedOutput = new Jimp(32, 32, function (err, sortedImage) {
      sortedColors = _.sortBy(simpleColors);
      for (var i = 0; i < 32; i++) {
        for (var j = 0; j < 32; j++) {
          sortedImage.setPixelColor(sortedColors[i + 32*j], i, j);
        }
      }
      sortedImage.write(imagesLocation + "sortedImage.bmp");
    });

    var setOutput = new Jimp(19, 19, function (err, setImage) {
      var arr = _.map(_.keys(colorSet), function (c) {
        return parseInt(c)<<8 + 0xff;
      });
      for (var i = 0; i < 19; i++) {
        for (var j = 0; j < 19; j++) {
          if(i + 19*j < arr.length)
            setImage.setPixelColor(arr[i + 19*j], i, j);
        }
      }
      setImage.write(imagesLocation + "setImage.bmp");
    });

    colorSettoCppMap = _.reduce(colorSet, function(result, value, key) {
        return result += '\tstd::make_pair(' + key + ',' + value + '),\n';
    }, "#include <map>\n\nstatic const std::map<int, int>::value_type x[] = { \n");
    colorSettoCppMap += "\n}; \n\nstatic const std::map<int, int> CONSOLE_COLOR_MAP(x, x + sizeof x / sizeof x[0]);";
    fs.writeFile("../Renderer/ConsoleColorMap.h", colorSettoCppMap);

    colorSettoCppArray = _.reduce(colorSet, function(result, value, key) {
        return result += "\t\t" + key + ", \n";
    }, "static const int COLOR_LIST_SIZE = "+_.keys(colorSet).length+";\nstatic const int COLOR_LIST[COLOR_LIST_SIZE] = { \n");
    colorSettoCppArray += "};";
    fs.writeFile("../Renderer/ConsoleColorList.h", colorSettoCppArray);
});
