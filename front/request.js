function postData(host){
    const Http = new XMLHttpRequest();
    const url=`${host}/dogfeeder`;
    Http.open("POST", url);
    Http.send(JSON.stringify({teste: 'teste'}));

    let executedFunction = false;
    Http.onreadystatechange = (e) => {
        if(!executedFunction && Http.responseText){
            executedFunction = true
            console.log(Http.responseText)
        }
    }
}  

postData('http://192.168.0.14:8080');

function getData(host){
    const Http = new XMLHttpRequest();
    const url=`${host}/dogfeeder`;
    Http.open("GET", url);
    Http.send();

    let executedFunction = false;
    Http.onreadystatechange = (e) => {
        if(!executedFunction && Http.responseText){
            executedFunction = true
            console.log(JSON.parse(Http.responseText).map(a=>JSON.parse(a)))
        }
    }
}

getData('http://192.168.0.14:8080');