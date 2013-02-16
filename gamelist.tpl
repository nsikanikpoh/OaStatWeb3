{{%AUTOESCAPE context="HTML"}}
<table border="1">
<tr><td>Game number</td><td>Gametype</td><td>Map name</td><td>Mod name</td><td>Server name</td><td>Start time</td></tr>
{{#ELEMENT}}
<tr><td><a href='../game/{{GAMENUMBER}}'>{{GAMENUMBER}}</a></td><td>{{GAMETYPE}}</td><td><a href='../map/{{MAPNAME}}'>{{MAPNAME}}</a></td><td>{{BASEGAME}}</td><td>{{SERVERNAME}}</td><td>{{TIMESTAMP}}</td></tr>
{{/ELEMENT}}
</table>