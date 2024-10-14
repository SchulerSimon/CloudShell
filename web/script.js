let path = '/'
// History to keep track of previous commands
let commandHistory = []
let historyIndex = 0

// get input field
const inputField = document.getElementById('input')

// function to focus the input field
function focusInput () {
  inputField.value = path + ' $:'
  inputField.focus()
}

// focus input field when loading
window.onload = focusInput

// focus input field when clicking anywhere
document.addEventListener('click', focusInput)

//
document.addEventListener('DOMContentLoaded', function () {
  const outputDiv = document.getElementById('output')
  const fileInput = document.getElementById('fileInput')

  const listOfCommands = [
    'help\n\t\t- prints this screen',
    'clear\n\t\t- clears the screen',
    'ls\n\t\t- list all files and folders'
    // 'download [filename]\n\t\t- download a file',
    // 'upload [filename]\n\t\t- open upload dialog',
    // 'mv [source] [dest]\n\t\t- move a file from source to dest',
    // "grep [string or regex]\n\t\t- list only files who's name contains string or regex",
    // 'rm [source]\n\t\t- delete the file',
    // 'cd [dest]\n\t\t- change the current working directory to dest'
  ]

  const apiUrl = 'http://localhost:50505/'

  // A simple command processor
  function processCommand (command) {
    command = command.split(' $:', 2)[1]
    if (command == 'clear') {
      outputDiv.innerHTML = ''
      return
    }
    if (command == 'help') {
      appendToOutput(
        'List of all supported commands:\n\t' +
          listOfCommands.toString().replace(new RegExp(',', 'g'), '\n\t')
      )
      return
    }
    if (command == 'upload') {
      fileInput.click()
      return
    }
    let temp = command.split(' ', 2)
    command = temp[0]
    let params = temp[1]
    console.log(command, params)
    fetch(apiUrl + 'command/' + command, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        path: path,
        params: params || ''
      })
    }).then(response => {
      if (response.ok) {
        response.json().then(json => {
          appendToOutput(json['answer'])
          path = json['path']
          inputField.value = path + ' $:'
        })
      }
    })
  }

  function appendToOutput (text) {
    text = text.replace(/\n/g, '<br>')
    text = text.replace(/\t/g, '&nbsp;&nbsp;&nbsp;&nbsp;')
    const p = document.createElement('p')
    p.innerHTML = text // Use innerHTML to render the HTML
    outputDiv.appendChild(p)
    outputDiv.scrollTop = outputDiv.scrollHeight // Scroll to the bottom
  }

  inputField.addEventListener('keydown', function (e) {
    if (e.key === 'Enter') {
      const command = inputField.value.trim()
      if (command) {
        appendToOutput(`> ${command}`)
        commandHistory.push(command)
        historyIndex = commandHistory.length
        processCommand(command)
        inputField.value = ''
      }
    } else if (e.key === 'ArrowUp') {
      if (historyIndex > 0) {
        historyIndex--
        inputField.value = commandHistory[historyIndex]
      }
    } else if (e.key === 'ArrowDown') {
      if (historyIndex < commandHistory.length - 1) {
        historyIndex++
        inputField.value = commandHistory[historyIndex]
      } else {
        inputField.value = ''
        historyIndex = commandHistory.length
      }
    }
  })
})
