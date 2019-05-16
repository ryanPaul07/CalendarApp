let global_fileNames = [] // Global variable to hold all the valid files
let fileNameGlobal       // Variable used to hold the file selected from second table
let login = false;
let storeAllFiles = false;
let lOfFiles = 0;

// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

    // Button to clear Status Panel
    $('#clearButton').click(function(e){
      $('#Status-T-Body').children().empty();
    });

    // Button to clear Status Panel
    $('#clearButtonConsole').click(function(e){
      $('#Console-T-Body').children().empty();
    });

    // Prompt to make user use valid file endings
    $("#fileName").change(function () {
        var fileExtension = ['ics'];
        if ($.inArray($(this).val().split('.').pop().toLowerCase(), fileExtension) == -1) {
            alert("Only formats are allowed : "+fileExtension.join(', '));
        }
    });

    // Checks if UTC is clicked for start
    $("#utcStart").click(function(e){
      $(this).data('clicked', true);
    });

    // Checks if UTC is clicked for creation
    $("#utcCreation").click(function(e){
      $(this).data('clicked', true);
    });

    // Checks if UTC is clicked for start
    $("#utcStartEvent").click(function(e){
      $(this).data('clicked', true);
    });

    // Checks if UTC is clicked for creation event
    $("#utcStampEvent").click(function(e){
      $(this).data('clicked', true);
    });

    // Makes the scroll bar always go to the bottom of the table in the status panel
    window.setInterval(function() {
      var elem = document.getElementById('div-Status');
      elem.scrollTop = elem.scrollHeight;
    }, 5000);

    // Makes the scroll bar always go to the bottom of the table in the status panel
    window.setInterval(function() {
      var elem = document.getElementById('div-Console');
      elem.scrollTop = elem.scrollHeight;
    }, 70000);

    // Checks if Login Button is clicked
    $("#LoginButton").click(function(e){
      let loginName = $('#Username').val();
      let loginPassword = $('#Password').val();
      let loginDBName = $('#DbName').val();


      console.log(loginName);
      console.log(loginPassword);
      console.log(loginDBName);

      if(loginName.length == 0 || loginPassword.length == 0 || loginDBName .length == 0)
      {
        alert("Fill in all properties");
      }
      else
      {
        console.log("success!!!");

        login = true;

        $.ajax({
            type: 'get',            //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/login',   //The server endpoint we are connecting to
            data: {
              loginNameSent : loginName,
              loginPassword : loginPassword,
              loginDBName : loginDBName,
            },
            success: function (data) {

              console.log(data);
              if(data == 0)
              {
                alert("Login failed: please enter valid credentials");
              }
              else if(data == 1)
              {

                console.log("success  before =============");


                $.ajax({
                    type: 'get',            //Request type
                    //dataType: 'json',       //Data type - we will use JSON for almost everything
                    url: '/createTable',   //The server endpoint we are connecting to
                    success: function (data) {
                      console.log("test 2121"+data);
                      if(data == 1)
                      {
                        console.log("good job success !!!");
                      }
                    },
                    fail: function(error) {
                        // Non-200 return, do something with error
                        console.log(error);
                    }
                });

                console.log("after ajax");
                alert("Login successufully: created tables FILE, EVENT and ALARM| if additional files are uploaded pleases login again");
                console.log("success  after =============");
              }
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });

      }
    });


    // Clear data from the table
    $("#clearData").click(function(e){
      if(login == true)
      {
        $.ajax({
            type: 'get',            //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/clearTable',   //The server endpoint we are connecting to
            success: function (data) {
              console.log("cleared alll data");
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });

        $.ajax({
            type: 'get',            //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/displayDB',   //The server endpoint we are connecting to
            success: function (data) {
              console.log(data);
              let obj = JSON.parse(data);
              //console.log(obj.fileLength);
              $('#Console-T-Body').append("<tr><th> Database has "+obj.fileLength+" files, "+obj.eventLength+" events, and "+obj.alarmLength+" alarms <tr><th>");
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });


      }
      else
      {
        alert("Must be logged in");
      }
    });

    // Display data from the table
    $("#displayDB").click(function(e){
      if(login == true)
      {
        $.ajax({
            type: 'get',            //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/displayDB',   //The server endpoint we are connecting to
            success: function (data) {
              console.log(data);
              let obj = JSON.parse(data);
              //console.log(obj.fileLength);
              $('#Console-T-Body').append("<tr><th> Database has "+obj.fileLength+" files, "+obj.eventLength+" events, and "+obj.alarmLength+" alarms <tr><th>");
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });
      }
      else
      {
        alert("Must be logged in");
      }
    });

    // Execute queries from the table
    $("#eventsSD").click(function(e){
      if(login == true)
      {
        if(storeAllFiles == true)
        {
          console.log("reached the event start date");
          $.ajax({
              type: 'get',            //Request type
              dataType: 'json',       //Data type - we will use JSON for almost everything
              url: '/orderByStartDate',   //The server endpoint we are connecting to
              success: function (data) {
                //console.log(data.start_time);

                // let obj = (data);
                // console.log(JSON.parse(obj[0]).event_id);
                let obj = data;
                for(var a = 0; a < obj.length; a++)
                {
                   $('#Console-T-Body').append("<tr><th> start_time: "+JSON.parse(obj[a]).start_time+", location: "+JSON.parse(obj[a]).location+", summary: "+JSON.parse(obj[a]).summary+", event_id: "+JSON.parse(obj[a]).event_id+", cal_file "+JSON.parse(obj[a]).cal_file+"  <tr><th>");
                }
              },
              fail: function(error) {
                  // Non-200 return, do something with error
                  console.log(error);
              }
          });
        }
        else
        {
          alert("Please store all files first");
        }
      }
      else
      {
        alert("Must be logged in");
      }
    });

    // Execute queries from the table
    $("#eventSF").click(function(e){

      let fileMatch = 0;
      if(login == true)
      {
        let fileName = $('#databaseFile').val();
        console.log(fileName);

        for(let f = 0; f < global_fileNames.length; f++)
        {
          if(fileName == global_fileNames[f])
          {
            fileMatch = 1;
            break;
          }
        }

        console.log("reached after the for loop");

        if(fileMatch == 1)
        {
          $.ajax({
              type: 'get',            //Request type
              //dataType: 'json',       //Data type - we will use JSON for almost everything
              url: '/displaySpecificFile',   //The server endpoint we are connecting to
              data: {
                dataBaseFile: fileName,
              },
              success: function (data) {

                let obj = data;
                for(var a = 0; a < obj.length; a++)
                {
                   $('#Console-T-Body').append("<tr><th> start_time: "+JSON.parse(obj[a]).start_time+", summary: "+JSON.parse(obj[a]).summary+", event_id: "+JSON.parse(obj[a]).event_id+" <tr><th>");
                }

              },
              fail: function(error) {
                  // Non-200 return, do something with error
                  console.log(error);
              }
          });
        }
        else
        {
          alert("File does not exist in Database");
        }
      }
      else
      {
        alert("Must be logged in");
      }
    });

    // Execute queries from the table
    $("#eventWC").click(function(e){
      if(login == true)
      {

      }
      else
      {
        alert("Must be logged in");
      }
    });

    // Store data into the queries from file log table
    $("#storeF").click(function(e){
      if(login == true)
      {
        if(lOfFiles == 0)
        {
          alert("No files present please add files");
        }
        else
        {
          storeAllFiles = true;
          console.log("Reached storing files in database");
          $.ajax({
              type: 'get',            //Request type
              //dataType: 'json',       //Data type - we will use JSON for almost everything
              url: '/storeToDatabase',   //The server endpoint we are connecting to
              data: {
                files: global_fileNames,
              },
              success: function (data) {

              },
              fail: function(error) {
                  // Non-200 return, do something with error
                  console.log(error);
              }
          });

          $.ajax({
              type: 'get',            //Request type
              //dataType: 'json',       //Data type - we will use JSON for almost everything
              url: '/displayDB',   //The server endpoint we are connecting to
              success: function (data) {
                console.log(data);
                let obj = JSON.parse(data);
                //console.log(obj.fileLength);
                $('#Console-T-Body').append("<tr><th> Database has "+obj.fileLength+" files, "+obj.eventLength+" events, and "+obj.alarmLength+" alarms <tr><th>");
              },
              fail: function(error) {
                  // Non-200 return, do something with error
                  console.log(error);
              }
          });
        }
      }
      else
      {
        alert("Must be logged in");
      }
    });



    $('#submitEvent').click(function(e){

      let startDate = $('#dtStartDateEvent').val().split("-",3).join('');
      let startTime = $('#dtStartTimeEvent').val().split(':',2).join('').concat("00");
      let creationDate = $('#dtStampDateEvent').val().split("-",3).join('');
      let creationTime = $('#dtStampTimeEvent').val().split(':',2).join('').concat("00");
      let uid = $('#uid').val();
      let summary = $('#Summary').val();
      console.log(fileNameGlobal);

      if(fileNameGlobal == null || startDate.length == 0 || startTime.length == 0 || creationDate.length == 0 || creationTime.length == 0 || uid.length == 0)
      {
        if(fileNameGlobal == null)
        {
          alert("Please select a file");
        }
      }
      else
      {
        if($("#utcStartEvent").data('clicked'))
        {
          startTime = startTime.concat("Z");
        }


        if($("#utcStampEvent").data('clicked'))
        {
          creationTime = creationTime.concat("Z");
        }

        console.log(startDate);
        console.log(startTime);
        console.log(creationDate);
        console.log(creationTime);
        console.log(summary);
        console.log(uid);


        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/addEvent',   //The server endpoint we are connecting to
            data: {
              file: fileNameGlobal,
              dtStartDate: startDate,
              dtStartTime: startTime,
              dtStampDate: creationDate,
              dtStampTime: creationTime,
              uiD: uid,
              Summary: summary,
            },
            success: function (data) {


              console.log(data);
              var i;
              var obj
              obj = data;
              //console.log(obj.version);
              if(obj.errorMessage == null)
              {
                //$('#File-Log-Table').append("<tr>"+"<th><a href=\"/uploads/"+obj.fileName+"\" download>"+obj.fileName+"</a>"+"</th><th>"+obj.version+"</th><th>"+obj.prodID+"</th><th>"+obj.numEvents+"</th><th>"+obj.numProps+"</th></tr>");
                $('#Status-T-Body').append("<tr><th> successufully added event to  \""+obj.fileName+"\" -- Reload page to view <tr><th>");

                //$('#firstDropDownId').append("<a onclick=\"myFunction("+i+")\" class=\"dropdown-item\" href=\"#\">"+obj.fileName+"</a>");
                //global_fileNames.push(obj.fileName);
              }
              else
              {
                $('#Status-T-Body').append("<tr><th> while adding event to  "+obj.fileName+" ,"+obj.errorMessage+" error occured <tr><th>");
              }

              //location.reload();

            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });
      }
    });



    $('#calendarButton').click(function(e){

      console.log('test this is reached 11!!!!');
      let fileName = $('#fileName').val();
      let version = $('#Version').val();
      let prodid = $('#ProdiD').val();
      let dtstartDate = $('#dtStartDate').val().split("-",3).join('');
      let dtstartTime = $('#dtStartTime').val().split(':',2).join('').concat("00");
      let dtstampDate = $('#dtStampDate').val().split("-",3).join('');
      let dtstampTime = $('#dtStampTime').val().split(":",2).join('').concat("00");
      let uid = $('#UID').val();

      e.preventDefault();

      if(fileName.length == 0 || version.length == 0 || dtStartDate.length == 0 || dtStartTime.lenght == 0 || dtStampTime.length == 0 || dtStampDate.length == 0 || uid.length == 0 || isNaN(version))
      {

      }
      else
      {
        if($("#utcStart").data('clicked'))
        {
          dtstartTime = dtstartTime.concat("Z");
        }


        if($("#utcCreation").data('clicked'))
        {
          dtstampTime = dtstampTime.concat("Z");
        }
        // let dtStartDate2 = dtStartDate.concat("T");

        //console.log(dtStartDate2);
        //let dtStart = dtStartDate2.concat(dtStampTime);
        console.log(fileName);
        console.log(version);
        console.log(prodid);
        console.log(dtstartDate);
        console.log(dtstartTime);
        console.log(dtstampDate);
        console.log(dtstampTime);
        console.log(uid);
        //console.log(dtStart);

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/createCalendar',   //The server endpoint we are connecting to
            data: {
              file: fileName,
              ver: version,
              prodiD: prodid,
              dtStartDate: dtstartDate,
              dtStartTime: dtstartTime,
              dtStampDate: dtstampDate,
              dtStampTime: dtstampTime,
              uiD: uid,
            },
            success: function (data) {

              var i;
              var obj
              obj = data;

              if(obj.errorMessage == null)
              {
                $('#Status-T-Body').append("<tr><th> -- successufully created \""+obj.fileName+"\" -- Reload page to view <tr><th>");
              }
              else
              {
                $('#Status-T-Body').append("<tr><th>  ====== while validating "+obj.fileName+" "+obj.errorMessage+" error occured  ====== <tr><th>");
              }
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });
      }
    });



    // file log panel
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/object',   //The server endpoint we are connecting to
        // data: {
        //   fileSelected:obj.fileName;
        // },
        success: function (data) {

          var i;
          var obj
          var increment = 0;

          if(data.length  == 0)
          {
            $('#File-Log-Table').append("<tr id=\"NoFile\"><th> No File </tr>,/th>");
          }
          else
          {
            $('#NoFile').remove();
          }

          for (i = 0; i < data.length; i++) {
            obj = JSON.parse(data[i]);
            //console.log(obj.version);
            if(obj.errorMessage == null)
            {
              $('#File-Log-Table').append("<tr>"+"<th><a href=\"/uploads/"+obj.fileName+"\" download>"+obj.fileName+"</a>"+"</th><th>"+obj.version+"</th><th>"+obj.prodID+"</th><th>"+obj.numEvents+"</th><th>"+obj.numProps+"</th></tr>");
              $('#Status-T-Body').append("<tr><th> successufully uploaded \""+obj.fileName+"\" <tr><th>");

              $('#firstDropDownId').append("<a onclick=\"myFunction("+i+")\" class=\"dropdown-item\" href=\"#\">"+obj.fileName+"</a>");
              $('#secondDropDown').append("<button onclick=\"myFunctionTwo("+i+")\" class=\"dropdown-item\">"+obj.fileName+"</button>");
              global_fileNames[i] = obj.fileName;
              console.log(global_fileNames[i]+": "+i);
              increment++;
            }
            else
            {
              $('#Status-T-Body').append("<tr><th> ====== while validating "+obj.fileName+" "+obj.errorMessage+" error occured ====== <tr><th>");
            }
          }
          lOfFiles = increment;
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });


    //isFileExists();

});

// function isFileExists() {
//
//   // files in uploads
//   $.ajax({
//       type: 'get',            //Request type
//       dataType: 'text',       //Data type - we will use JSON for almost everything
//       url: '/fileInUploads',   //The server endpoint we are connecting to
//       success: function (data) {
//
//         console.log(data.lenght);
//         if(data.lenght == undefined || data.length  == 0 || data.length == null)
//         {
//           $('#File-Log-Table').append("<tr id=\"NoFile\"><th> No files </th></tr>");
//           isNoFile = true;
//         }else
//           {
//             console.log("test reached");
//             $('#NoFile').remove();
//
//           }
//       },
//       fail: function(error) {
//           // Non-200 return, do something with error
//           console.log(error);
//       }
//   });
// }






function myFunctionTwo(selected) {

  fileNameGlobal = global_fileNames[selected];

}

function myFunction(selected) {

  //var test = data;
  console.log(selected);
  console.log(global_fileNames[selected]);

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',
      data:{fileName: global_fileNames[selected]},       //Data type - we will use JSON for almost everything
      url: '/selectedToTable',   //The server endpoint we are connecting to
      success: function (data) {
      var i;
      var objEvent = data;

      $('#Calendar-View-Table').find('tbody').empty();

      console.log(objEvent);
      for (i = 1; i < data.length; i++) {

        var hours = objEvent[i].startDT.time.slice(0,2);
        var mins = objEvent[i].startDT.time.slice(2,4);
        var sec = objEvent[i].startDT.time.slice(4,6);

        var year = objEvent[i].startDT.date.slice(0,4);
        var month = objEvent[i].startDT.date.slice(4,6);
        var date = objEvent[i].startDT.date.slice(6,8);

        if(objEvent[i].startDT.isUTC == true)
        {
          $('#Calendar-View-Table').append("<tr><th>"+i+"</th><th>"+year+"/"+month+"/"+date+"</th><th>"+hours+":"+mins+":"+sec+" (UTC) "+"</th><th>"+objEvent[i].summary+"</th><th>"+" <button id=\"props"+i+"\"  onclick=\"showProp("+i+","+selected+")\" type=\"button\" class=\"btn btn-outline-secondary\">"+objEvent[i].numProps+"</button>"+"</th><th>"+" <button id=\"alarms"+i+"\"  onclick=\"showAlarm("+i+","+selected+")\" type=\"button\" class=\"btn btn-outline-secondary\">"+objEvent[i].numAlarms+"</button>"+"</th></tr>");
        }
        else
        {
          $('#Calendar-View-Table').append("<tr><th>"+i+"</th><th>"+year+"/"+month+"/"+date+"</th><th>"+hours+":"+mins+":"+sec+"</th><th>"+objEvent[i].summary+"</th><th>"+" <button id=\"props"+i+"\"   onclick=\"showProp("+i+","+selected+")\" type=\"button\" class=\"btn btn-outline-secondary\">"+objEvent[i].numProps+"</button>"+"</th><th>"+" <button id=\"alarms"+i+"\" onclick=\"showAlarm("+i+","+selected+")\"  type=\"button\" class=\"btn btn-outline-secondary\">"+objEvent[i].numAlarms+"</button>"+"</th></tr>");
        }
      }

      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });


}

// Displays Event properites in the Status Panel

function showProp(eventNum, selected) {
  console.log("this is inside the show props jquery function");
  console.log(eventNum);
  console.log(selected);
  console.log(global_fileNames[selected]);
  let file = global_fileNames[selected];
  console.log("RYaN" + file);

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',
      data:{
        fileName: file,
        eventNumber: eventNum
      },       //Data type - we will use JSON for almost everything
      url: '/showProp',   //The server endpoint we are connecting to
      success: function (data) {

        let i = 0;
        var objectProperty ; //= data;

        objectProperty = data;

        console.log("testreached insided ajax calll");
        console.log(data);

        $('#Status-T-Body').append("<tr><th> Displaying optional properties Event #"+eventNum+" </th></tr>");

        for (i = 0; i < data.length; i++) {

          console.log("clicked");
          if(objectProperty[i].zeroProperties == null)
          {
            $('#Status-T-Body').append("<tr><th>"+objectProperty[i].propName+":"+objectProperty[i].propDescription+"</th></tr>");
          }
          //if(objectAlarm[i].zeroAlarm == true)

          console.log("reached for loop");
        }

        if(objectProperty.zeroProperties == "true")
        {
          $('#Status-T-Body').append("<tr><th> There are no Properties  </th></tr>");
        }
      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });

}

// Displays Event alarms in the Status Panel

function showAlarm(eventNum, selected) {

  console.log("this is inside the show alarm jquery function");
  console.log(eventNum);
  console.log(global_fileNames[selected]);
  let file = global_fileNames[selected];

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',
      data:{
        fileName: file,
        eventNumber: eventNum
      },       //Data type - we will use JSON for almost everything
      url: '/showAlarm',   //The server endpoint we are connecting to
      success: function (data) {

        let i = 0;
        var objectAlarm ; //= data;

        objectAlarm = data;

        $('#Status-T-Body').append("<tr><th> Displaying alarms Event #"+eventNum+" </th></tr>");

        console.log(data);
        for (i = 0; i < data.length; i++) {

          console.log("Inside the for loop");
          if(objectAlarm[i].zeroAlarm == null)
          {
            $('#Status-T-Body').append("<tr><th> Action: "+objectAlarm[i].action+", Trigger: "+objectAlarm[i].trigger+", Num Props: "+objectAlarm[i].numProps+"</th></tr>");
          }
          //if(objectAlarm[i].zeroAlarm == true)

          console.log("reached for loop");
        }

        if(objectAlarm.zeroAlarm == "true")
        {
          $('#Status-T-Body').append("<tr><th> There are no Alarms  </th></tr>");
        }
      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });


}
