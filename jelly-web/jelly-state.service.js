
function switchStateRequest() {
  return makeGetRestCall("http://192.168.0.163/switch");
}

function getLightStateRequest(){
  return makeGetRestCall("http://192.168.0.163/getState")
}

function switchToPartyModeRequest(bpm){
  return makeGetRestCall(`http://192.168.0.163/switch?bpm=${bpm}`);
}


function makeGetRestCall(request) {
  if(jellyInSameNetwork){
    return fetch(request)
      .then((response) => {
        if (response.status === 200) {
          return response.json();
        } else {
          throw new Error("Something went wrong on API server!");
        }
      })
  } else {
    console.log(request);
    return new Promise((resolve, reject) => {
      setTimeout(() => {
        if(request.includes("bpm")){
          resolve({
            state: 2,
            party: true
          });
        }
        resolve({
          state: !lightState
        });
      }, 1000)
    })
  }
}
