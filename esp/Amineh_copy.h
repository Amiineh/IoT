const char index_html[] PROGMEM = R"rawliteral(
<html>
<head>
    <title>2x2 table grid</title>
    <style>
        table {
            border: 1px solid black;
        }
        td {
            border: 1px solid black;
            width: 40px;
            height: 40px
        }
        .cell {
            display: block;
            width: 40px;
            height: 40px
        }
        .color-cell {
            border: 0px;
            width: 40px;
            height: 40px;
            border-radius: 20px;
        }
    </style>
    <script language="JavaScript" type="text/javascript">
        var color = '';
        function setDivStyle(id) {
            var cell = document.getElementById(id);
            cell.style.backgroundColor = color;
        }
        function setColor(color) {
            this.color = color;
        }
    </script>
</head>
<body>
<div id="r1">
    <h2>Color plate </h2>
    <table id="color-plate" cellpadding="0" cellspacing="0" border="0px">
        <td class="color-cell" style="background-color: aqua "><a class="cell"
                                                                  href="javascript:setColor('#00FFFF')"></a></td>
        <td class="color-cell" style="background-color: blue "><a class="cell"
                                                                  href="javascript:setColor('#0000FF')"></a></td>

    </table>
    <table id="table" cellpadding="0" cellspacing="0">
        <br>
        <tr>
            <td id="10"><a class="cell" href="javascript:setDivStyle('10')"></a></td>
            <td id="11"><a class="cell" href="javascript:setDivStyle('11')"></a></td>
            <td id="12"><a class="cell" href="javascript:setDivStyle('12')"></a></td>
            <td id="13"><a class="cell" href="javascript:setDivStyle('13')"></a></td>
            <td id="14"><a class="cell" href="javascript:setDivStyle('14')"></a></td>
            <td id="15"><a class="cell" href="javascript:setDivStyle('15')"></a></td>
            <td id="16"><a class="cell" href="javascript:setDivStyle('16')"></a></td>
            <td id="17"><a class="cell" href="javascript:setDivStyle('17')"></a></td>
            <td id="18"><a class="cell" href="javascript:setDivStyle('18')"></a></td>
            <td id="19"><a class="cell" href="javascript:setDivStyle('19')"></a></td>
        </tr>
    </table>
</div>
</body>
</html>
)rawliteral";
