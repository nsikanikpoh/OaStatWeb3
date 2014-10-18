{{%AUTOESCAPE context="HTML"}}
<table border="1">
<tr><th>Player</th><th>Score</th></tr>
{{#ELEMENT}}
<tr><td><a href='../player/{{PLAYERID}}'>{{NICKNAME}}</a></td><td>{{SCORE}}</td></tr>
{{/ELEMENT}}
</table>
