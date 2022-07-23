let count =0;
function counter()
{
    document.querySelector('h1').innerHTML= count+=1;
}

document.addEventListener('DOMContentLoaded',function()
{
    document.querySelector('button').onclick = counter;
    setInterval(counter,1000);

});