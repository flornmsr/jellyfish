function setJellyState(switchState) {

  const colorOn = ["#ff9f63", "#ff5555", "#ffb380"]
  const colorOff = ["#2e2e2d", "#1c1c1c", "#4b769c"]

  var currentColor = colorOff;

  const svg = document.getElementById("svg5108");

  if (switchState == 1) {
    currentColor = colorOn;
    svg.classList.add("svg5108-glow");
  } else {
    svg.classList.remove("svg5108-glow");
  }

  const grad1 = document.getElementById("stop5846");
  const grad2 = document.getElementById("stop5848");
  const tentacles = document.getElementById("tentacles");
  const path = document.getElementById("path5775");


  grad1.setAttribute("stop-color", currentColor[0]);
  grad2.setAttribute("stop-color", currentColor[1]);
  tentacles.setAttribute("stroke", currentColor[1]);
  path.setAttribute("fill", currentColor[2]);
  // svg.style.filter = val.checked ? "#glow" : "";
}

function switchChanged() {
  loading(true);
  switchStateRequest()
    .then(data => {
      loading(false);
      console.log(data.state);
      console.log(lightState);
      if (data.state == lightState) {
        setSwitchButtonState(data.state);
      }
      lightState = data.state;
      console.log(`new state: ${lightState}`);
      setJellyState(lightState);
    })
    .catch((error) => {
      loading(false);
      console.error(error);
      setSwitchButtonState(lightState)
    });
}

function setSwitchButtonState(state) {
  var button = document.getElementById("switch");
  if (state == 0) {
    console.log("Button OFF");
    button.checked = false;
  } else {
    console.log("Button ON");
    button.checked = true;
  }
}

function loading(value) {
  isLoading = value;
  toggleEnable();
}

function toggleEnable() {
  const button = document.getElementById("switch");

  button.disabled = isLoading;
}

window.onload = (event) => {
  getLightStateRequest()
    .then(data => {
      loading(false);
      lightState = data.state;
      console.log(`Light state: ${lightState}`);
      setSwitchButtonState(lightState);
      setJellyState(lightState)
    })
    .catch((error) => {
      loading(false);
      console.error(error);
    });
};
