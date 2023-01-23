pipeline {
  agent any

  stages {
    stage("dagger") {
      steps {
        cd dagger
        go run main.go
      }
    }
  }
}
