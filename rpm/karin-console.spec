Name: karin-console
Version: 0.2.5harmattan3
Release: 1
Summary: Konsole on MeeGo Harmattan.
Group: System/Base
License: GPLv2
Source0: %{name}.tar.gz
URL: https://github.com/glKarin/karinconsole4nemo
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Widgets)
Provides: meego-terminal

%description
%{summary}.
 Terminal emulator for MeeGo Harmattan.
 Based on MeeGo-Terminal.

%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_libdir}/*
%{_datadir}/applications/*.desktop
%{_datadir}/karin-console/resource/*
%{_datadir}/karin-console/i18n/*
%{_datadir}/karin-console/toolbars/*
%{_datadir}/karin-console/icon/*

%prep
%setup -n meegoterminal4nemo-master


%build
qmake -qt=5 MEEGO_EDITION=nemo
# Inject version number from RPM into source
make %{?_smp_mflags}


%install


