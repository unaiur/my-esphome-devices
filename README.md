This repo contains a list of my ESPHome scripts

# Compile
`
esphome compile lector-contador-s2.yaml
`

# Upload
`
esphome run lector-contador-s2.yaml
`

# Fetch logs
esphome logs lector-contador-s2.yaml

# Setup DNS in ESPHome

./shell esphome
ping dns
echo "nameserver <ip>" > /etc/resolv.conf
