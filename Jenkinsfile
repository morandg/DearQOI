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

		stage('Check for new tags') {
			steps {
				script {
					env.CURRENT_TAG = sh(returnStdout: true, script: 'git tag --contains | head -1').trim()
				}
			}
		}

		stage('Publish to github') {
			when {
				expression {
					env.CURRENT_TAG != null && env.CURRENT_TAG != ''
				}
			}
			steps {
				echo "Creating github release ${env.CURRENT_TAG}"
				withCredentials([string(credentialsId: 'morandg-github-token', variable: 'GH_TOKEN')]) {
					sh 'scripts/create-release.sh --token ${GH_TOKEN} --owner morandg --repo DearQOI --tag ${CURRENT_TAG} --artifacts build/DearQOI'
				}
			}
		}
	}
}
