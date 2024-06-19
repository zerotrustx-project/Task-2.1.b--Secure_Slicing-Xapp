export ORAN_REPO=release

export DEBIAN_FRONTEND=noninteractive
export TZ=America/Chicago

# O-RAN version-specific package support: set to `latest` to
# get the latest stuff upstream in packagecloud $ORAN_REPO.
export ORAN_VERSIONS=
export MDCLOG_VERSION=0.1.1-1
export RMR_VERSION=4.4.6

apt-get update &&
	apt-get install -y cmake vim python3-pip wget g++ libssl-dev rapidjson-dev git \
		ca-certificates curl gnupg apt-transport-https apt-utils \
		pkg-config autoconf libtool libcurl4-openssl-dev &&
	curl -s https://packagecloud.io/install/repositories/o-ran-sc/${ORAN_REPO}/script.deb.sh | os=debian dist=stretch bash &&
	(
		[ "${ORAN_VERSIONS}" = "latest" ] ||
			apt-get install -y \
				mdclog=$MDCLOG_VERSION mdclog-dev=$MDCLOG_VERSION \
				rmr=$RMR_VERSION rmr-dev=$RMR_VERSION &&
			apt-get install -y \
				mdclog mdclog-dev \
				rmr rmr-dev
	) &&
	rm -rf /var/lib/apt/lists/*

wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz &&
	tar xf release-1.8.0.tar.gz &&
	cd googletest-release-1.8.0 &&
	cmake -DBUILD_SHARED_LIBS=ON . &&
	make &&
	cp -a googletest/include/gtest /usr/include &&
	cp -a googlemock/gtest/libgtest_main.so googlemock/gtest/libgtest.so /usr/lib/ &&
	make install &&
	ldconfig -v | grep gtest

cd /tmp &&
	git clone https://gitlab.flux.utah.edu/powderrenewpublic/xapp-frame-cpp &&
	cd xapp-frame-cpp &&
	mkdir -p build && cd build &&
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DDEV_PKG=1 .. &&
	make && make install && ldconfig &&
	cd /tmp && rm -rf /tmp/xapp-frame-cpp

cd /tmp &&
	git clone https://gitlab.flux.utah.edu/powderrenewpublic/asn1c-eurecom asn1c &&
	cd asn1c &&
	git checkout f12568d617dbf48497588f8e227d70388fa217c9 &&
	autoreconf -iv &&
	./configure &&
	make install &&
	ldconfig &&
	cd .. &&
	rm -rf /tmp/asn1c

cd /tmp &&
	git clone https://gitlab.flux.utah.edu/powderrenewpublic/pistache &&
	cd pistache && mkdir build && cd build &&
	cmake ../ && make install && ldconfig &&
	cd .. && rm -rf /tmp/pistache

cd /tmp &&
	git clone https://github.com/offa/influxdb-cxx &&
	cd influxdb-cxx &&
	git checkout 6b76bd02f26166e03888214914e5f9a000feb7d8 &&
	mkdir -p build && cd build &&
	cmake ../ && make install && ldconfig &&
	cd .. && rm -rf /tmp/influxdb-cxx

cp -r ./* /nexran

cd /nexran &&
	rm -rf build && mkdir build && cd build &&
	([ ! -e /nexran/lib/e2ap/messages/e2ap-v01.00.asn1 ] &&
		mkdir -p /nexran/lib/e2ap/messages/generated &&
		curl https://www.emulab.net/downloads/johnsond/profile-oai-oran/E2AP-v01.00-generated-bindings.tar.gz | tar -xzv -C /nexran/lib/e2ap/messages/generated &&
		echo "RIC_GENERATED_E2AP_BINDING_DIR:STRING=/nexran/lib/e2ap/messages/generated/E2AP-v01.00" >>CMakeCache.txt) ||
	true &&
	([ ! -e /nexran/lib/e2sm/messages/e2sm-kpm-v01.00.asn1 ] &&
		mkdir -p /nexran/lib/e2sm/messages/generated &&
		curl https://www.emulab.net/downloads/johnsond/profile-oai-oran/E2SM-KPM-ext-generated-bindings.tar.gz | tar -xzv -C /nexran/lib/e2sm/messages/generated &&
		echo "RIC_GENERATED_E2SM_KPM_BINDING_DIR:STRING=/nexran/lib/e2sm/messages/generated/E2SM-KPM" >>CMakeCache.txt) ||
	true &&
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ../ &&
	make install && ldconfig

export RMR_RTG_SVC="9999"
export RMR_SEED_RT="/nexran/etc/routes.txt"
export DEBUG=1
export XAPP_NAME="nexran"
export XAPP_ID="1"

cd /nexran/build/src && ctest
