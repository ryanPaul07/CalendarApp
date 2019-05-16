'use strict'

// C library API
const ffi = require('ffi');
let connection = null;
// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');
const mysql = require('mysql');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;


  try{

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
}catch(err){
  res.send("No File");
}
});



//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ********************

//sharedLib calendar object
let sharedLib = ffi.Library('./libcal',{
  'objectToJson' : ['string',['string']],
  'GUIToCalendar' : ['string',['string','string','string','string','string','string','string','string']],
  'eventObjectToJson' : ['string',['string']],
  'alarmListToJSON' : ['string',['string','string']],
  'addEventToCalendar' : ['string',['string','string','string','string','string','string','string']],
  'eventPropertyListToJSON' : ['string',['string','string']],
});

// calendar object to json
app.get('/object', function(req , res){

  let files = [];
  files = fs.readdirSync('./uploads/');
  let array = [];
  for(var i =0; i< files.length; i++)
  {
    array[i] = sharedLib.objectToJson(files[i]);
  }
  res.send(array);
});

// checks if there are files in the uploads folder
app.get('/fileInUploads', function(req , res){

  let files = [];
  files = fs.readdirSync('./uploads/');
  console.log("/fileUploads = "+files);
  res.send(files);
});


// add Event to Calendar
app.get('/addEvent', function(req , res){

  let files = req.query.file;
  let dtstartdate = req.query.dtStartDate;
  let dtstarttime = req.query.dtStartTime;
  let dtstampdate = req.query.dtStampDate;
  let dtstamptime = req.query.dtStampTime;
  let uid = req.query.uiD;
  let summary = req.query.Summary;
  let output = sharedLib.addEventToCalendar(files, dtstartdate, dtstarttime, dtstampdate, dtstamptime, uid, summary);
  res.send(output);

});




app.get('/selectedToTable', function(req , res){

  let file = sharedLib.eventObjectToJson(req.query.fileName);
  res.send(file);

});


app.get('/showAlarm', function(req , res){

  let displayMessage = sharedLib.alarmListToJSON(req.query.fileName, req.query.eventNumber);
  console.log(displayMessage);
  res.send(displayMessage);

});

app.get('/showProp', function(req , res){

  let displayMessage = sharedLib.eventPropertyListToJSON(req.query.fileName, req.query.eventNumber);
  console.log(displayMessage);
  res.send(displayMessage);

});


app.get('/createCalendar', function(req , res){

  let files = req.query.file;
  let version = req.query.ver;
  let prodid = req.query.prodiD;
  let dtstartdate = req.query.dtStartDate;
  let dtstarttime = req.query.dtStartTime;
  let dtstampdate = req.query.dtStampDate;
  let dtstamptime = req.query.dtStampTime;
  let uid = req.query.uiD;
  let output = sharedLib.GUIToCalendar(files, version, prodid, dtstartdate, dtstarttime, dtstampdate, dtstamptime, uid);
  res.send(output);

});

// ================================================ Assignment 4 Functionality ====================================

app.get('/login', function(req , res){

  let loginName = req.query.loginNameSent;
  let loginPassword = req.query.loginPassword;
  let loginDBName = req.query.loginDBName;

  connection = mysql.createConnection({
    host : 'dursley.socs.uoguelph.ca',
    user : loginName,
    password : loginPassword,
    database : loginDBName
  });

  connection.connect(function(err) {
    if (err)
    {
      res.send("0");
      console.log("invalid credintials");
    }
    else
    {
      res.send("1");
      console.log("success created connection");
    }
  });
  //create table
});

app.get('/createTable', function(req , res){

  var fileTable = "CREATE TABLE FILE (cal_id INT NOT NULL AUTO_INCREMENT, file_Name VARCHAR(60) NOT NULL, version INT NOT NULL, prod_id VARCHAR(256) NOT NULL, PRIMARY KEY(cal_id) );";
  var eventTable = "CREATE TABLE EVENT (event_id INT NOT NULL AUTO_INCREMENT, summary VARCHAR(1024), start_time DATETIME NOT NULL, location VARCHAR(60), organizer VARCHAR(256), cal_file INT NOT NULL, PRIMARY KEY(event_id), FOREIGN KEY(cal_file) REFERENCES FILE(cal_id) ON DELETE CASCADE );";
  var alarmTable = "CREATE TABLE ALARM (alarm_id INT NOT NULL AUTO_INCREMENT, action VARCHAR(256) , `trigger` VARCHAR(256) , event INT NOT NULL, PRIMARY KEY(alarm_id), FOREIGN KEY(event) REFERENCES EVENT(event_id) ON DELETE CASCADE );";

  connection.query(fileTable, function (err, rows, fields) {
    if (err)
    {
      console.log("Something went wrong. "+err);
    }
  });

  connection.query(eventTable, function (err, rows, fields) {
    if (err)
    {
      console.log("Something went wrong. "+err);
    }
  });

  connection.query(alarmTable, function (err, rows, fields) {
    if (err)
    {
      console.log("Something went wrong. "+err);
    }
  });

  console.log("test reached end")

  res.send("1");
});

app.get('/clearTable', function(req , res){

  connection.query("delete from FILE", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
        console.log("Cleared data from table");
  });

  connection.query("delete from EVENT", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
        console.log("Cleared data from table");
  });

  connection.query("delete from ALARM", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
        console.log("Cleared data from table");
  });

});

app.get('/storeToDatabase', function(req , res){

  var fileNames = req.query.files;
  var lenghtOfFile = fileNames.length;
  var arrayFile = [];
  var arrayEvent = [];
  // var arrayAlarm = [];


  // Clear the ALARM table;
  connection.query("delete from ALARM", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
        console.log("Cleared data from table");
  });

  // Clear the EVENT table;
  connection.query("delete from EVENT", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
        console.log("Cleared data from table");
  });

  // Clear the FILE table;
  connection.query("delete from FILE", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
        console.log("Cleared data from table");
  });

  console.log("reached store to database ajax call");

  for(var i =  0; i < lenghtOfFile; i++)
  {
    arrayFile[i] = JSON.parse(sharedLib.objectToJson(fileNames[i]));
    arrayEvent[i] = JSON.parse(sharedLib.eventObjectToJson(fileNames[i]));

  }


  console.log(arrayFile[0]);
  console.log("===========");
  console.log(arrayFile[1]);
  console.log("===========");
  console.log(arrayFile[2]);

  console.log("------------");
  console.log(arrayEvent[0]);
  console.log("------------");
  console.log("------------");
  console.log(arrayEvent[1]);
  console.log("------------");
  console.log("------------");
  console.log(arrayEvent[2]);
  console.log("------------");

  console.log("length of file is = " + lenghtOfFile);


  for(var i = 0; i < lenghtOfFile; i++)
  {
    console.log("FileName: "+arrayFile[i].fileName+" version: "+arrayFile[i].version+" prodid: "+arrayFile[i].prodID);
  }

  console.log("============================");



  for(var i =  0; i < lenghtOfFile; i++)
  {
    connection.query("INSERT INTO FILE (file_Name , version , prod_id) VALUES ('"+arrayFile[i].fileName+"', '"+arrayFile[i].version+"', '"+arrayFile[i].prodID+"')", function (err, rows, fields) {
      if(err) console.log("Something went wrong. " + err);
      else
      {
        console.log("Success: Inserted into FILE Table index = "+ i);
      }
    });
  }


  console.log("============================");

  //--------------------------- STORING INTO EVENT -------------------

  connection.query("SELECT * FROM FILE ORDER BY cal_id", function(err, rows, fields) {
    if(err) console.log("Something went wrong. " + err);
    else
    {
      console.log("Database contents sorted by cal_id:");
      // for the rows in the FILE table
      for (let row of rows)
      {
        let eventTable = JSON.parse(sharedLib.eventObjectToJson(row.file_Name));

        console.log("-------------");
        console.log(eventTable);
        console.log("-------------");
        console.log("row = "+row.cal_id);

        for(var i = 1; i < eventTable.length; i++)
        {
          let numOfProps = JSON.parse(sharedLib.eventPropertyListToJSON(row.file_Name, String(i)));
          let alarmTable = JSON.parse(sharedLib.alarmListToJSON(row.file_Name, String(i)));

          let summary = null;
          let location = null;
          let organizer = null;

          var hours = eventTable[i].startDT.time.slice(0,2);
          var mins = eventTable[i].startDT.time.slice(2,4);
          var sec = eventTable[i].startDT.time.slice(4,6);

          var year = eventTable[i].startDT.date.slice(0,4);
          var month = eventTable[i].startDT.date.slice(4,6);
          var date = eventTable[i].startDT.date.slice(6,8);

          let dateF = year + "-" + month + "-" + date;
          let timeF = hours + ":" + mins + ":" + sec;

          let datetimeF = dateF + " " + timeF;
          console.log(datetimeF);

          let fileName = row.file_Name;
          console.log(fileName);
          console.log(i);


          for(var j = 0; j < numOfProps.length; j++)
          {
            if(numOfProps[j].propName == "SUMMARY")
            {
              summary = numOfProps[j].propDescription;
            }
            if(numOfProps[j].propName == "ORGANIZER")
            {
              organizer = numOfProps[j].propDescription;
            }
            if(numOfProps[j].propName == "LOCATION")
            {
              location = numOfProps[j].propDescription;
            }
          }




          console.log(i);

          connection.query("INSERT INTO EVENT (summary, start_time, location, organizer, cal_file) VALUES ('"+summary+"', '"+datetimeF+"', '"+location+"', '"+organizer+"', '"+row.cal_id+"')", function (err, rows, fields) {
            if(err) console.log("Something went wrong. " + err);
            else
            {
              console.log("Success inserted into event: index = " + i);

            //   if(row)
            //   {
            //
            //
            //     if(alarmTable.length > 0)
            //     {
            //       for(let q = 0; q < alarmTable.length; q++)
            //       {
            //         if(alarmTable[q].zeroAlarm == null)
            //         {
            //           connection.query("INSERT INTO ALARM (action, `trigger`, event) VALUES ('"+alarmTable[q].action+"', '"+alarmTable[q].trigger+"', '"+row.event_id+"')", function (err, rows, fields) {
            //             if(err) console.log("Something went wrong. " + err);
            //             else
            //             {
            //               console.log("Success inserted into alarm: index = " + i);
            //             }
            //
            //           });
            //         }
            //       }
            //     }
            //     else
            //     {
            //       console.log("there are no alarms for this event");
            //     }
            // }





              // if(eventTable[i].numAlarms > 0)
              // {
              //   for(var a = 0; a < eventTable[i].numAlarms; a++)
              //   {
              //     connection.query("SELECT event_id FROM EVENT WHERE ");
              //     connection.query("INSERT INTO ALARM (action, `trigger`, event) VALUES ('"+alarmTable[a].action+"', '"+alarmTable[a].trigger+"', '"+i+"')", function (err, rows, fields) {
              //       if(err) console.log("Something went wrong. " + err);
              //       else
              //       {
              //         console.log("Success inserted into alarm: index = " + i);
              //       }
              //
              //     });
              //   }
              // }
            }

          });

        }
      }
    }
  });



//-----------------------------------------------------------------

  // --------------------------- inserting into ALARM table ---------------



  connection.query("SELECT * FROM FILE ORDER BY cal_id", function(err, rows, fields) {
    if(err) console.log("Something went wrong. " + err);
    else
    {
      for (let row of rows)
      {
        let fileNameTwo = row.file_Name;
        let fileId = row.cal_id;
        //console.log(fileNameTwo);
        connection.query("SELECT * FROM EVENT WHERE cal_file = "+fileId+"", function(err, rows, fields) {
          let x = 1;
          let alarmTableNew = JSON.parse(sharedLib.alarmListToJSON(fileNameTwo,String(x)));
          console.log(alarmTableNew);
          if(err) console.log("Something went wrong. " + err);
          else
          {
            console.log("Database contents sorted by event_id:");

            for(let row of rows)
            {
              console.log(row);
              x++;
              console.log("test reached pls");
              for(var a = 0; a < alarmTableNew.length; a++)
              {
                if(alarmTableNew.zeroAlarm == null)
                {
                  connection.query("INSERT INTO ALARM (action, `trigger`, event) VALUES ('"+alarmTableNew[a].action+"', '"+alarmTableNew[a].trigger+"', '"+row.event_id+"')", function (err, rows, fields) {
                      if(err) console.log("Something went wrong. " + err);
                      else
                      {
                        console.log("Success inserted into alarm: index = " + i);
                      }
                  });
                }
                else
                {
                  console.log("no alarms present");
                }
              }

            }
          }
        });
      }
    }
  });

  res.send("1");
});


app.get('/displayDB', function(req , res){

  let lengthOfFile = 0;
  let lengthOfEvent = 0;
  let lengthOfAlarm = 0;

  connection.query("SELECT * FROM FILE", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
    {
      console.log(rows.length);
      lengthOfFile = rows.length;
      connection.query("SELECT * FROM EVENT", function (err, rows, fields) {
        if (err)
            console.log("Something went wrong. ======= "+err);
        else
        {
          console.log(rows.length);
          lengthOfEvent = rows.length;
          connection.query("SELECT * FROM ALARM", function (err, rows, fields) {
            if (err)
                console.log("Something went wrong. ======= "+err);
            else
            {
              console.log(rows.length);
              lengthOfAlarm = rows.length;

              res.send("{ \"fileLength\":"+lengthOfFile+", \"eventLength\":"+lengthOfEvent+", \"alarmLength\":"+lengthOfAlarm+" }");
            }
          });
        }

      });
    }

  });
});

app.get('/orderByStartDate', function(req , res){
  let temp = []
  connection.query("SELECT * FROM EVENT ORDER BY start_time", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
    {
      for(let row of rows)
      {
        temp.push(JSON.stringify(row));
      }
      res.send(temp);
    }
  });


});

app.get('/displaySpecificFile', function(req , res){
  let tempFile = req.query.dataBaseFile;
  let temp2 = []
  console.log(tempFile);
  connection.query("SELECT * FROM FILE WHERE file_Name = \'"+tempFile+"\'", function (err, rows, fields) {
    if (err)
        console.log("Something went wrong. ======= "+err);
    else
    {
      for(let row of rows)
      {
        connection.query("SELECT * FROM EVENT WHERE cal_file = "+row.cal_id+"", function (err, rows, fields) {
          if (err)
              console.log("Something went wrong. ======= "+err);
          else
          {
            for(let row of rows)
            {
              temp2.push(JSON.stringify(row));
            }
            res.send(temp2);
          }
        });
      }

    }
  });


});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
