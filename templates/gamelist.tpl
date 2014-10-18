{{%AUTOESCAPE context="HTML"}}
<table border="1">
<tr><th>Game number</th><th>Gametype</th><th>Map name</th><th>Mod name</th><th>Server name</th><th>Start time</th></tr>
{{#ELEMENT}}
<tr><td><a href='../game/{{GAMENUMBER}}'>{{GAMENUMBER}}</a></td><td><a href='../gametype/{{GAMETYPE}}'>{{GAMETYPE_NAME}}</a></td><td><a href='../map/{{MAPNAME}}'>{{MAPNAME}}</a></td><td>{{BASEGAME}}</td><td>{{SERVERNAME}}</td><td>{{TIMESTAMP}}</td></tr>
{{/ELEMENT}}
</table>
