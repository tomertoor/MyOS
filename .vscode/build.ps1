$id = docker run --rm -d -it -v "${pwd}:/root/env" magshios
docker exec $id make build-x86_64
docker stop $id
qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso