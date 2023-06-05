#!/bin/sh
set -e

gh_token=
gh_owner=
gh_repo=
tag=
artifacts=

usage() {
  echo "${0} --token <token> --owner <owner> --repo <repo> --tag <tag> --artifacts \"<file1> <file2>\":"
  echo "-h|--help       Show this help"
  echo "-T|--token      Github application token"
  echo "-o|--owner      Github project owner"
  echo "-r|--repo       Github repository"
  echo "-t|--tag        Release tag name"
  echo "-a|--artifacts  List of artifacts"
}

while [ ${#} -gt 0 ] ; do
  case $1 in
    -h|--help)
      usage ${0}
      exit 0
      ;;
    -T|--token)
      gh_token=${2}
      shift 2
      ;;
    -o|--owner)
      gh_owner=${2}
      shift 2
      ;;
    -r|--repo)
      gh_repo=${2}
      shift 2
      ;;
    -t|--tag)
      tag=${2}
      shift 2
      ;;
    -a|--artifacts)
      artifacts=${2}
      shift 2
      ;;
    *)
      echo "Unkown argument: ${1}"
      usage ${0}
      exit 1
      ;;
  esac
done

if [ -z "${gh_token}" \
     -o -z "${gh_owner}" \
     -o -z "${gh_repo}" \
     -o -z "${tag}" \
     -o -z "${artifacts}"] ; then
  echo "Missing parameter!"
  usage ${0}
  exit 1
fi

# https://docs.github.com/en/rest/releases/releases?apiVersion=2022-11-28#create-a-release
echo "Creating release ${tag} ..."
release_json=$(curl --silent -XPOST \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer ${gh_token}"\
  -H "X-GitHub-Api-Version: 2022-11-28" \
  --data "{\"tag_name\":\"${tag}\",\"name\":\"${tag}\"}" \
  https://api.github.com/repos/${gh_owner}/${gh_repo}/releases)

release_id=$(echo ${release_json} | jq .id)
if [ ${release_id} = "null" ] ; then
  echo "Could not create release:"
  echo "${release_json}"
  exit 1
fi

if [ -e RELEASE_NOTES.md ] ; then
  echo "Uploading release notes ..."
  body=$(cat RELEASE_NOTES.md | sed --null-data --expression='s,",\\\",g' --expression='s,\n,\\n,g')
  release_notes_json=$(curl --silent -X POST \
    -H "Accept: application/vnd.github+json" \
    -H "Authorization: Bearer ${gh_token}"\
    -H "X-GitHub-Api-Version: 2022-11-28" \
    --data "{\"body\":\"${body}\"}" \
    https://api.github.com/repos/${gh_owner}/${gh_repo}/releases/${release_id})
fi

# https://docs.github.com/en/rest/releases/assets?apiVersion=2022-11-28#upload-a-release-asset
for artifact_path in ${artifacts} ; do
  echo "Uploading ${artifact_path}"
  artifact_json=$(curl --silent -XPOST \
    -H "Accept: application/vnd.github+json" \
    -H "Authorization: Bearer ${gh_token}"\
    -H "X-GitHub-Api-Version: 2022-11-28" \
    -H "Content-Type: application/octet-stream" \
    --data-binary "@${artifact_path}" \
    "https://uploads.github.com/repos/${gh_owner}/${gh_repo}/releases/${release_id}/assets?name=$(basename ${artifact_path})")
  upload_status=$(echo ${artifact_json} | jq .state)
  if [ ${upload_status} = "null" ] ; then
    echo "Could not upload artifact:"
    echo "${artifact_json}"
    exit 1
  fi
done

echo "Release ${tag} created!"
