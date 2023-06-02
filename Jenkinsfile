pipeline {
	agent {
		dockerfile {
			filename 'Dockerfile'
			dir 'docker'
		}
	}

	stages {
		stage('Build') {
			steps {
				sh './docker/build.sh'
			}
		}

		stage('Publish artifacts') {
			steps {
				dir("${env.WORKSPACE}") {
					sh "echo -n"
				}
				archiveArtifacts(artifacts: "build/DearQOI")
			}
		}
	}
}
