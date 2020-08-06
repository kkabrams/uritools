#!/bin/bash
[ "$1" ] || exit 1
uri="$1"
if [ "${uri}" != "${uri:0:64}" ];then
  prompt="${uri:0:64}..."
fi
scheme="$(uricut -s <<< "$uri")"
selection="$(printf "start\ncopy\nshorten\nverbose\nforget\n%s\n" "$(printf "%s\n" "$uri" | uricut | grep -v '^whole')" \
             | choose "${prompt}")"
#            | choose "$(printf "uri: '%s'\nwill be ran with: %s\n" "$uri" "$(urigetline "$uri")" )" -default start )"
echo "$selection"
if [ "$selection" = "verbose" ];then
  x-terminal-emulator -e less -f <(printf "%s" "$uri")
  exit 0
fi
if [ "$selection" = "copy" ];then
  printf "%s" "$uri" | xclip -i
  exit 0
fi
if [ "$selection" = "forget" ];then
  cp ~/.cache/uristart.log ~/.cache/uristart.log.tmp
  cat ~/.cache/uristart.log.tmp | grep -vFw "${uri}" > ~/.cache/uristart.log
  exit 0
fi
if [ "$selection" = "start" ];then
  scheme="$(printf "%s\n" "$uri" | uricut -s)"
  exec uristart "$uri" "$(xdotool getwindowfocus getwindowname)"
  exit 0
fi
if [ "$selection" = "shorten" ];then
  escaped="$(uriescape "$uri")"
  gemini-get "gemini://epo.k.vu/submit?$escaped" | tr -d '\r' | tail -n+2 | head -n2 | tr '\n' '\0' | xargs -n1 -0 copy_start_nevermind.sh
  exit 0
fi
if [ "$selection" ];then
  exec printf "%s" "$selection" | cut '-d ' -f2- | xclip -i
fi
