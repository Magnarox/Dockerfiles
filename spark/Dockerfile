FROM openjdk:8u232-jdk

RUN apt update

# Install Ruby
RUN apt install git curl libssl-dev libreadline-dev zlib1g-dev autoconf bison build-essential libyaml-dev libreadline-dev libncurses5-dev libffi-dev libgdbm-dev -y
RUN curl -sL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-installer | bash -; exit 0
ENV PATH /root/.rbenv/shims:/root/.rbenv/bin:/root/.rbenv/plugins/ruby-build/bin:$PATH
RUN rbenv install 2.5.1
RUN rbenv global 2.5.1
RUN gem install jekyll jekyll-redirect-from rouge

# Install Python 3.7.1
RUN apt install build-essential checkinstall libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev -y
COPY ./files/Python-3.7.1.tgz /tmp/
RUN chmod 755 /tmp/Python-3.7.1.tgz && tar -xzf /tmp/Python-3.7.1.tgz -C /tmp/
RUN cd /tmp/Python-3.7.1 && ./configure --enable-optimizations && make altinstall
RUN update-alternatives --install /usr/bin/pip pip /usr/local/bin/pip3.7 1
RUN update-alternatives --install /usr/bin/python python /usr/local/bin/python3.7 1
RUN update-alternatives --install /usr/bin/pydoc pydoc /usr/local/bin/pydoc3.7 1
RUN pip install --upgrade pip && pip install sphinx pypandoc mkdocs

# Install R
RUN apt install r-base libcurl4-openssl-dev libxml2-dev -y
RUN Rscript -e 'install.packages(c("knitr", "devtools", "rmarkdown", "e1071"), repos="https://cloud.r-project.org/")'
RUN Rscript -e 'devtools::install_version("roxygen2", version = "5.0.1", repos="https://cloud.r-project.org/")'
RUN Rscript -e 'devtools::install_version("testthat", version = "1.0.2", repos="https://cloud.r-project.org/")'

# Install Docs Stuff
RUN apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra qpdf -y

# Add Spark 2.4.4 src
COPY ./files/spark-2.4.4.tgz /tmp/
RUN mkdir /spark/ && mkdir /spark/out/
RUN chmod 755 /tmp/spark-2.4.4.tgz && tar -xzf /tmp/spark-2.4.4.tgz -C /spark/ 

WORKDIR /spark/

COPY ./files/build-distrib.sh /spark/
RUN chmod 755 /spark/build-distrib.sh

