FROM disconnect3d/nsjail
WORKDIR /jail
COPY ./jail .
RUN chmod -R 755 .
CMD nsjail -Ml --port 9003 --disable_proc --chroot /jail -- /bin/hangpwn