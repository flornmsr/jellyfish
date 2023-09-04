function switchToPartyMode() {
  let inputValue = document.getElementById("party").value;
  let validationError = document.getElementById("validation-error");

  if(inputValue == '' || inputValue<0 || inputValue > 1000){
    validationError.style.display = 'block';
    return;
  }
  validationError.style.display = 'none';

  loading(true);
  switchToPartyModeRequest(inputValue).then(value => {
    loading(false);
    lightState = value.state;
    partyModeUpdate(value.party)

  }).catch((error) => {
    loading(false);
  })}
  function partyModeUpdate(isPartyMode){
    let gif = document.getElementById("sponge-bob");
    let input = document.getElementById("party");
    let label = document.getElementById("partyLabel");
    let partyButton = document.getElementById("partyBtn");
    let stopPartyButton = document.getElementById("stopPartyBtn");


    displayElement(gif, isPartyMode);
    displayElement(input, !isPartyMode);
    displayElement(label, !isPartyMode);
    displayElement(partyButton, !isPartyMode);
    displayElement(stopPartyButton, isPartyMode);
  }

  function displayElement(element, show){
    element.style.display = show ? 'block' : 'none';

  }

 function stopPartyMode() {
    partyModeUpdate(false);
 }

  function loading(value) {
    isLoading = value;
    disableButton();
  }

  function disableButton() {
    let button = document.getElementById("partyBtn");
    button.disabled = isLoading;
  }


  const buttonLabel = {
    isParty: "Fertig!",
    noParty: "Okeyyy, Lets gooo!"
  }

window.onload = (event) => {
  console.log('party')
};

