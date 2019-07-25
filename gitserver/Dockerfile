FROM centos:7.6.1810

RUN yum install git httpd -y

RUN mkdir /var/www/git \
  && mkdir /var/www/git/parser.git \
  && cd /var/www/git/parser.git \
  && git init --bare \
  && chown -R apache:apache /var/www

RUN echo $'\n[http] \n\
    receivepack = true' >> /var/www/git/parser.git/config

RUN rm -R /etc/httpd/conf.d/*

ADD ./git.conf /etc/httpd/conf.d/git.conf
ADD ./entrypoint.sh /entrypoint.sh

RUN chmod 755 entrypoint.sh

EXPOSE 80

ENTRYPOINT ["/entrypoint.sh"]

