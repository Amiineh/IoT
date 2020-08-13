function tableCreate() {
    var body = document.getElementsByTagName('body')[0];
    body.style.textAlign='center';
    var tbl = document.createElement('table');
    tbl.setAttribute('border', '1');
    tbl.style.textAlign='center';
    var tbdy = document.createElement('tbody');
    for (var i = 0; i < 12; i++) {
        var tr = document.createElement('tr');
        for (var j = 0; j < 10; j++) {
            var td = document.createElement('td');
            var id = j*12 - i + 11;
            td.innerHTML= '<a id= '+id+' class="cell" href="javascript:setDivStyle(\''+ String(id) +'\')" onclick=\'setDivStyle(' + id + ');\'\'></a>';
            // td.innerHTML= '<a id= '+id+' class="cell" onclick=setDivStyle(\''+ String(id) +'\')></a>';
            td.style.textAlign='center';
            tr.appendChild(td)
        }
        tbdy.appendChild(tr);
    }
    tbl.appendChild(tbdy);
    body.appendChild(tbl);
}

tableCreate();