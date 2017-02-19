{{%AUTOESCAPE context="HTML"}}
<html>
<head>

<link rel="stylesheet" type="text/css" media="screen" href="{{STATIC_MEDIA}}/css/ui-lightness/jquery-ui-1.10.3.custom.css" />
<link rel="stylesheet" type="text/css" media="screen" href="{{STATIC_MEDIA}}/css/ui.jqgrid.css" />
 
<style type="text/css">
html, body {
    margin: 0;
    padding: 0;
    font-size: 75%;
}
</style>


<script src="{{STATIC_MEDIA}}/js/jquery-1.9.0.min.js" type="text/javascript"></script>
<script src="{{STATIC_MEDIA}}/js/i18n/grid.locale-en.js" type="text/javascript"></script>
<script src="{{STATIC_MEDIA}}/js/jquery.jqGrid.min.js" type="text/javascript"></script>
<script src="{{STATIC_MEDIA}}/oastat_formatters.js" type="text/javascript"></script>
<script src="{{STATIC_MEDIA}}/oastat_common.js" type="text/javascript"></script>

<script type="text/javascript">
$(function () {
    $("#list").jqGrid({
        url: "datasource/killsbyweapon",
        datatype: "json",
        mtype: "GET",
        colNames: ["Weapon", "Kills"],
        colModel: [
            { name: "w",  formatter:weaponFormatter },
            { name: "c" }
        ],
        pager: "",
        sortname: "c",
        sortorder: "desc",
        viewrecords: true,
        gridview: true,
        autoencode: true,
        height: "100%",
        caption: "Kills by weapon"
    }); 
}); 

</script>

<title>{{TITLE}} - {{SUBTITLE}}</title>
{{META_TAGS}}
</head>
<body  bgcolor="silver">
<div id="container" style="text-align:center;">
<center>
<H1>{{TITLE}}</H1>
<H2>{{SUBTITLE}}</H2>
<div id="topmenu" style="background-color:#FFA500;">
<a href="{{ROOTPATH}}/.">Summary</a> <a href="{{ROOTPATH}}/gamelist/0">Gamelist</a>
</div>
{{#BODY_ELEMENT_LIST}}
<div id="element" >
<h2>{{ELEMENT_TITLE}}</h2>
{{BODY_ELEMENT:none}}
</div>
{{/BODY_ELEMENT_LIST}}
<div id="element" >
<h2>Kills by weapon</h2>
<div id="chart_div" style="width:400; height:300"></div>
</div>
<div id="element" >
<h2>Kills by weapon</h2>
<table id="list"><tr><td></td></tr></table> 
 <div id="pager"></div> 
</div>
</center>
</div>



</body>
</html>


<script language="javascript" type="text/javascript" 
        src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.0/jquery.min.js">
    </script>

<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<script type="text/javascript">
// Load the Visualization API and the piechart package.
      google.load('visualization', '1.0', {'packages':['corechart']});
     
      // Set a callback to run when the Google Visualization API is loaded.
      google.setOnLoadCallback(drawChart);


      // Callback that creates and populates a data table, 
      // instantiates the pie chart, passes in the data and
      // draws it.
      function drawChart() {

      // Create the data table.
      var jsonData = $.ajax({
                url: "datasource/killsbyweapon?sidx=2&sord=DESC",
                dataType: "json",
                async: false
            }).responseText;

            var data = new google.visualization.DataTable();
      data.addColumn('string', 'Weapon');
      data.addColumn('number', 'Kills');

      obj = jQuery.parseJSON(jsonData);
      var rows = obj.rows;
      for( var i=0; i<rows.length; i++ ) {
          data.addRow([weaponFormatter(rows[i].cell[0]),Number(rows[i].cell[1])]);
      };

      // Set chart options
      var options = {'title':'Weapon kills by weapon',
                     'width':500,
                     'height':400};

      // Instantiate and draw our chart, passing in some options.
      var chart = new google.visualization.PieChart(document.getElementById('chart_div'));
      chart.draw(data, options);
    }

</script>